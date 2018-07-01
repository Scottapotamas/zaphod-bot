/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"
#include "eui_serial_transport.h"

#include "hal_uuid.h"
#include "app_version.h"

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
	const char * build_branch;
	const char * build_info;
	const char * build_date;
	const char * build_time;
} BuildInfo_t;

typedef struct
{
	uint16_t speed_rpm;
	uint8_t setpoint_percentage;
	uint8_t state;
} FanData_t;

typedef struct
{
	uint8_t temperature;
	uint8_t percentage;
} FanCurve_t;

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

	uint8_t	mode;	//manual, demo, program execute, etc

	//information about the current move being executed
	uint8_t profile_type;
	uint8_t move_progress;
} MotionData_t;

typedef struct
{
	bool enabled;
	uint8_t state;
	uint8_t feedback;
	float angle;
	float target;
	float current;
	float power;
} MotorData_t;

SystemData_t 	sys_stats;
BuildInfo_t		fw_info;
InternalInterface_t internal_comm_modes;
InternalIO_t	internal_io_modes;
ExternalIO_t	external_io_modes;

FanData_t 		fan_stats;
FanCurve_t 		fan_curve[5];
TempData_t 		temp_sensors;

MotionData_t 	motion_global;
MotorData_t 	motion_servo[4];

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

};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
configuration_init( void )
{
	//perform any setup here if needed


}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_set_defaults( void )
{
	//set build info to hardcoded values
	fw_info.build_branch 	= ProgramBuildBranch;
	fw_info.build_info 		= ProgramBuildInfo;
	fw_info.build_date 		= ProgramBuildDate;
	fw_info.build_time 		= ProgramBuildTime;

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
	setup_identifier( HAL_UUID, 12 );	//header byte is 96-bit, therefore 12-bytes
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
config_set_movement_data( uint8_t move_type, uint8_t progress )
{
	motion_global.profile_type = move_type;
	motion_global.move_progress = progress;
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
config_motor_current( uint8_t servo, float amps )
{
	motion_servo[servo].current = amps;
}

PUBLIC void
config_motor_angle( uint8_t servo, float angle )
{
	motion_servo[servo].angle = angle;
}

/* ----- Private Functions -------------------------------------------------- */



/* ----- End ---------------------------------------------------------------- */
