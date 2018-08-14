#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"
#include "eui_serial_transport.h"

#include "hal_uuid.h"
#include "app_version.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "app_events.h"
#include "app_signals.h"

typedef struct
{
	bool sensors_enable;	//if ADC sampling and conversions are active
	bool module_enable;		//add-in card powered

	//microcontroller info
	uint8_t cpu_load;	//percentage
	uint8_t cpu_clock;	//speed in Mhz
	float cpu_temp;		//temp in degrees

	float input_voltage;	//voltage
} SystemData_t;

typedef struct
{
	uint8_t mode_group_0;	//UART
	uint8_t mode_group_1;	//I2C
} InternalInterface_t;

typedef struct
{
	uint8_t aux_0;	//PWM, IO capable
	uint8_t aux_1;	//PWM, IO capable
	uint8_t aux_2;	//PWM, IO capable
	uint8_t aux_3;	//DAC/ADC/IO capable
	uint8_t aux_4;  //DAC/ADC/IO capable
} InternalIO_t;

typedef struct
{
	uint8_t mode_group_0;	//uart capable IO pair
	uint8_t mode_group_1;	//CAN capable IO pair
	bool usb_enabled;
} ExternalIO_t;

typedef struct
{
	char build_branch[8];
	char build_info[12];
	char build_date[10];
	char build_time[8];
	char build_type[8];
} BuildInfo_t;

typedef struct
{
	uint16_t speed_rpm;
	uint8_t setpoint_percentage;
	uint8_t state;
} FanData_t;

typedef struct
{
	float pcb_ambient;
	float pcb_regulator;
	float external_probe;
} TempData_t;

typedef struct
{
	//global position of end effector in cartesian space
	int32_t x;
	int32_t y;
	int32_t z;

	//pathing engine state idle, running, etc
	uint8_t	pathing_state;

	//information about the current move being executed
	uint8_t movement_identifier;
	uint8_t profile_type;
	uint8_t move_progress;

	//motion handler information
	uint8_t	motion_state;
	uint8_t	motion_queue_depth;

} MotionData_t;

typedef struct
{
	bool enabled;
	uint8_t state;
	uint8_t feedback;
	float current_angle;
	float target_angle;
	float power;
} MotorData_t;

SystemData_t 	sys_stats;
BuildInfo_t		fw_info;
InternalInterface_t internal_comm_modes;
InternalIO_t	internal_io_modes;
ExternalIO_t	external_io_modes;

FanData_t 		fan_stats;
FanCurve_t 		fan_curve[] =
{
	{ .temperature = 0, .percentage =   20 },
    { .temperature = 20, .percentage =  20 },
    { .temperature = 35, .percentage =  45 },
    { .temperature = 45, .percentage =  90 },
    { .temperature = 60, .percentage = 100 },
};

TempData_t 		temp_sensors;

MotionData_t 	motion_global;
MotorData_t 	motion_servo[4];
Movement_t 		motion_inbound;

PRIVATE void emergency_stop_cb( void );
PRIVATE void home_system_cb( void );
PRIVATE void movement_generate_event( void );

PRIVATE void publish_motion_cb( void );
PRIVATE void publish_motion_cb_2( void );
PRIVATE void publish_motion_cb_l( void );
PRIVATE void publish_motion_cb_r( void );

uint8_t pingvar = 99;

euiMessage_t ui_variables[] =
{
    //higher level system setup information
    {.msgID = "sys", 	.type = TYPE_CUSTOM, .size = sizeof(SystemData_t),  .payload = &sys_stats       },
    {.msgID = "intDA", 	.type = TYPE_CUSTOM, .size = sizeof(InternalInterface_t), .payload = &external_io_modes },
    {.msgID = "intIO", 	.type = TYPE_CUSTOM, .size = sizeof(InternalIO_t),  .payload = &internal_io_modes   },
    {.msgID = "extIO", 	.type = TYPE_CUSTOM, .size = sizeof(ExternalIO_t),  .payload = &internal_comm_modes },
    {.msgID = "fwb", 	.type = TYPE_CUSTOM, .size = sizeof(BuildInfo_t), 	.payload = &fw_info      	},

    {.msgID = "fan", 	.type = TYPE_CUSTOM, .size = sizeof(FanData_t), 	.payload = &fan_stats      	},
    {.msgID = "curve", 	.type = TYPE_CUSTOM, .size = sizeof(fan_curve), 	.payload = &fan_curve  		},
    {.msgID = "temp", 	.type = TYPE_CUSTOM, .size = sizeof(TempData_t),  	.payload = &temp_sensors 	},

    //motion related information
    {.msgID = "moStat",	.type = TYPE_CUSTOM, .size = sizeof(MotionData_t), 	.payload = &motion_global 	},
    {.msgID = "mo1", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo[0] },
    {.msgID = "mo2", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo[1] },
    {.msgID = "mo3", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo[2] },
    {.msgID = "mo4", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo[3] },

	//inbound movement buffer and 'add to queue' callback
    {.msgID = "inmv", 	.type = TYPE_CUSTOM,   .size = sizeof(Movement_t), 				.payload = &motion_inbound },
    {.msgID = "qumv", 	.type = TYPE_CALLBACK, .size = sizeof(movement_generate_event), .payload = &movement_generate_event },

	//function callbacks
    {.msgID = "estop", 	.type = TYPE_CALLBACK, .size = sizeof(emergency_stop_cb),  	.payload = &emergency_stop_cb },
    {.msgID = "home", 	.type = TYPE_CALLBACK, .size = sizeof(home_system_cb),  	.payload = &home_system_cb },

	//test callbacks
#warning "Remove test movement calls once API surface matures"
    {.msgID = "tmove", 	.type = TYPE_CALLBACK, .size = sizeof(publish_motion_cb),  	.payload = &publish_motion_cb },
    {.msgID = "tmove2", 	.type = TYPE_CALLBACK, .size = sizeof(publish_motion_cb_2),  	.payload = &publish_motion_cb_2 },
    {.msgID = "lmove", 	.type = TYPE_CALLBACK, .size = sizeof(publish_motion_cb_l),  	.payload = &publish_motion_cb_l },
    {.msgID = "rmove", 	.type = TYPE_CALLBACK, .size = sizeof(publish_motion_cb_r),  	.payload = &publish_motion_cb_r },

	//temp variables
#warning "Remove temp variables once UI supports structure graphs"
    {.msgID = "power", 	.type = TYPE_FLOAT, .size = sizeof(motion_servo[1].power),  	.payload = &motion_servo[1].power },

	{.msgID = "ping", 	.type = TYPE_UINT8, .size = sizeof(pingvar),  	.payload = &pingvar },

};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
configuration_init( void )
{
	//perform any setup here if needed
	configuration_set_defaults();

}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_set_defaults( void )
{
	//set build info to hardcoded values
	memset(&fw_info.build_branch, 0, sizeof(fw_info.build_branch));
	memset(&fw_info.build_info, 0, sizeof(fw_info.build_info));
	memset(&fw_info.build_date, 0, sizeof(fw_info.build_date));
	memset(&fw_info.build_time, 0, sizeof(fw_info.build_time));
	memset(&fw_info.build_type, 0, sizeof(fw_info.build_type));

	strcpy(&fw_info.build_branch, ProgramBuildBranch );
	strcpy(&fw_info.build_info, ProgramBuildInfo );
	strcpy(&fw_info.build_date, ProgramBuildDate );
	strcpy(&fw_info.build_time, ProgramBuildTime );
	strcpy(&fw_info.build_type, ProgramBuildType );

	//Set the configurable IO to off
	internal_comm_modes.mode_group_0 	= PIN_INACTIVE;
	internal_comm_modes.mode_group_1 	= PIN_INACTIVE;
	external_io_modes.mode_group_0 		= PIN_INACTIVE;
	external_io_modes.mode_group_1 		= PIN_INACTIVE;
	external_io_modes.usb_enabled 		= false;

	internal_io_modes.aux_0 			= PIN_INACTIVE;
	internal_io_modes.aux_1 			= PIN_INACTIVE;
	internal_io_modes.aux_2 			= PIN_INACTIVE;
	internal_io_modes.aux_3 			= PIN_INACTIVE;
	internal_io_modes.aux_4 			= PIN_INACTIVE;



}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_load( void )
{
	//load settings from memory
	//todo implement flash storage mechanism

}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_save( void )
{
	//save settings to memory

}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_electric_setup( void )
{
	setup_identifier( (char*)HAL_UUID, 12 );	//header byte is 96-bit, therefore 12-bytes
	setup_dev_msg(ui_variables, ARR_ELEM(ui_variables));
}

/* -------------------------------------------------------------------------- */

// System Statistics and Settings

PUBLIC void
config_set_cpu_load( uint8_t percent )
{
	sys_stats.cpu_load = percent;
}

PUBLIC void
config_set_cpu_clock( float clock )
{
	sys_stats.cpu_clock = clock / 1000000;	//convert to Mhz
}

PUBLIC void
config_set_cpu_temp( float temp )
{
	sys_stats.cpu_temp = temp;
}

PUBLIC void
config_sensors_enable( bool enable )
{
	sys_stats.sensors_enable = enable;
}

void
config_module_enable(bool enabled)
{
	sys_stats.module_enable = enabled;
}

PUBLIC void
config_set_input_voltage( float voltage )
{
	sys_stats.input_voltage = voltage;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_fan_percentage( uint8_t percent )
{
	fan_stats.setpoint_percentage = percent;
}

PUBLIC void
config_set_fan_rpm( uint16_t rpm )
{
	fan_stats.speed_rpm = rpm;
}

PUBLIC void
config_set_fan_state( uint8_t state )
{
	fan_stats.state = state;
}

PUBLIC FanCurve_t*
config_get_fan_curve_ptr( void )
{
	if( DIM(fan_curve) == NUM_FAN_CURVE_POINTS  )
	{
		return &fan_curve[0];
	}

	return 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_temp_ambient( float temp )
{
	temp_sensors.pcb_ambient = temp;
}

PUBLIC void
config_set_temp_regulator( float temp )
{
	temp_sensors.pcb_regulator = temp;
}

PUBLIC void
config_set_temp_external( float temp )
{
	temp_sensors.external_probe = temp;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_position( int32_t x, int32_t y, int32_t z )
{
	motion_global.x = x;
	motion_global.y = y;
	motion_global.z = z;
}

PUBLIC void
config_set_movement_data( uint8_t move_id, uint8_t move_type, uint8_t progress )
{
	motion_global.movement_identifier = move_id;
	motion_global.profile_type 	= move_type;
	motion_global.move_progress = progress;
}

PUBLIC void
config_set_pathing_status( uint8_t status )
{
	motion_global.pathing_state = status;
}

PUBLIC void
config_set_motion_state( uint8_t status )
{
	motion_global.motion_state = status;
}

PUBLIC void
config_set_motion_queue_depth( uint8_t utilisation )
{
	motion_global.motion_queue_depth = utilisation;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_motor_enable( uint8_t servo, bool enable )
{
	motion_servo[servo].enabled = enable;
}

PUBLIC void
config_motor_state( uint8_t servo, uint8_t state )
{
	motion_servo[servo].state = state;
}

PUBLIC void
config_motor_feedback( uint8_t servo, uint8_t fb )
{
	motion_servo[servo].feedback = fb;
}

PUBLIC void
config_motor_power( uint8_t servo, float watts )
{
	motion_servo[servo].power = watts;
}

PUBLIC void
config_motor_current_angle( uint8_t servo, float angle )
{
	motion_servo[servo].current_angle = angle;
}

PUBLIC void
config_motor_target_angle( uint8_t servo, float angle )
{
	motion_servo[servo].target_angle = angle;
}

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void emergency_stop_cb( void )
{
	eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void home_system_cb( void )
{
	eventPublish( EVENT_NEW( StateEvent, MOTION_PREPARE ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void movement_generate_event( void )
{
	   MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

	   if(motion_request)
	   {
		   memcpy(&motion_request->move, &motion_inbound, sizeof(motion_inbound));
		   eventPublish( (StateEvent*)motion_request );
	   }
}

/* -------------------------------------------------------------------------- */

PRIVATE void publish_motion_cb( void )
{
	   MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

	   if(motev)
	   {
		   motev->move.type = _LINE;
		   motev->move.ref = _POS_RELATIVE;
		   motev->move.duration = 300;

		   //start
		   motev->move.points[0].x = 0;
		   motev->move.points[0].y = 0;
		   motev->move.points[0].z = 0;

		   //dest
		   motev->move.points[1].x = 0;
		   motev->move.points[1].y = 0;
		   motev->move.points[1].z = MM_TO_MICRONS(20);
		   motev->move.num_pts = 2;

		   eventPublish( (StateEvent*)motev );
	   }
}

PRIVATE void publish_motion_cb_2( void )
{
	   MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

	   if(motev)
	   {
		   motev->move.type = _LINE;
		   motev->move.ref = _POS_RELATIVE;
		   motev->move.duration = 300;

		   //start
		   motev->move.points[0].x = 0;
		   motev->move.points[0].y = 0;
		   motev->move.points[0].z = 0;

		   //dest
		   motev->move.points[1].x = 0;
		   motev->move.points[1].y = 0;
		   motev->move.points[1].z = MM_TO_MICRONS(-20);
		   motev->move.num_pts = 2;

		   eventPublish( (StateEvent*)motev );
	   }
}

PRIVATE void publish_motion_cb_r( void )
{
	   MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

	   if(motev)
	   {
		   motev->move.type = _LINE;
		   motev->move.ref = _POS_RELATIVE;
		   motev->move.duration = 300;

		   //start
		   motev->move.points[0].x = 0;
		   motev->move.points[0].y = 0;
		   motev->move.points[0].z = 0;

		   //dest
		   motev->move.points[1].x = MM_TO_MICRONS(20);
		   motev->move.points[1].y = 0;
		   motev->move.points[1].z = 0;
		   motev->move.num_pts = 2;

		   eventPublish( (StateEvent*)motev );
	   }
}

PRIVATE void publish_motion_cb_l( void )
{
	   MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

	   if(motev)
	   {
		   motev->move.type = _LINE;
		   motev->move.ref = _POS_RELATIVE;
		   motev->move.duration = 300;

		   //start
		   motev->move.points[0].x = 0;
		   motev->move.points[0].y = 0;
		   motev->move.points[0].z = 0;

		   //dest
		   motev->move.points[1].x = MM_TO_MICRONS(-20);
		   motev->move.points[1].y = 0;
		   motev->move.points[1].z = 0;
		   motev->move.num_pts = 2;

		   eventPublish( (StateEvent*)motev );
	   }
}

/* ----- End ---------------------------------------------------------------- */
