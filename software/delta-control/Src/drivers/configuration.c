#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"

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
	uint8_t supervisor;
	uint8_t motors;
	uint8_t control_mode;
} SystemStates_t;

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
	//pathing engine state idle, running, etc
	uint8_t	pathing_state;
    //motion handler information
    uint8_t	motion_state;
	//information about the current move being executed
	uint8_t profile_type;
	uint8_t move_progress;
    uint16_t movement_identifier;
} MotionData_t;

typedef struct
{
    uint8_t movements;
    uint8_t lighting;
} QueueDepths_t;

typedef struct
{
    uint8_t enabled;
	uint8_t state;
	uint8_t feedback;
	float target_angle;
	float power;
} MotorData_t;

typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint8_t enable;
} LedState_t;

typedef struct
{
    int16_t balance_red;
    int16_t balance_green;
    int16_t balance_blue;
    int16_t balance_total;
} LedSettings_t;


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

SystemStates_t	sys_states;
QueueDepths_t   queue_data;

MotionData_t 	motion_global;
MotorData_t 	motion_servo[4];
Movement_t 		motion_inbound;
CartesianPoint_t current_position; //global position of end effector in cartesian space
CartesianPoint_t target_position;

LedState_t    rgb_led_drive;
LedSettings_t rgb_led_settings;
Fade_t animation_inbound;

char device_nickname[16] = "Zaphod Beeblebot";

PRIVATE void start_mech_cb( void );
PRIVATE void stop_mech_cb( void );
PRIVATE void emergency_stop_cb( void );
PRIVATE void home_mech_cb( void );
PRIVATE void execute_motion_queue( void );
PRIVATE void pause_motion_queue_execution( void );
PRIVATE void clear_all_queue(void);

PRIVATE void tracked_position_event( void );
PRIVATE void movement_generate_event( void );
PRIVATE void lighting_generate_event( void );
PRIVATE void sync_begin_queues( void );

uint8_t sync_id_val = 0;
uint8_t mode_request = 0;

eui_message_t ui_variables[] =
{
    // higher level system setup information
    EUI_CUSTOM("sys", sys_stats),
    EUI_CHAR_RO_ARRAY("name", device_nickname),
    EUI_CUSTOM("super", sys_states),
    EUI_CUSTOM("fwb", fw_info),

    // Configuration of internal and external IO banks
    EUI_CUSTOM("intDA", external_io_modes),
    EUI_CUSTOM("intIO", internal_io_modes),
    EUI_CUSTOM("extIO", internal_comm_modes),

    // temperature and cooling system
    EUI_CUSTOM("fan", fan_stats),
    EUI_CUSTOM("curve", fan_curve),
    EUI_CUSTOM("temp", temp_sensors),

    EUI_CUSTOM( "queue", queue_data),

    // motion related information
    EUI_CUSTOM("moStat", motion_global),
    EUI_CUSTOM("mo1", motion_servo[0]),
    EUI_CUSTOM("mo2", motion_servo[1]),
    EUI_CUSTOM("mo3", motion_servo[2]),
#ifdef EXPANSION_SERVO
    EUI_CUSTOM("mo4", motion_servo[3]),
#endif

    EUI_CUSTOM_RO("rgb", rgb_led_drive ),
    EUI_CUSTOM_RO("ledset", rgb_led_settings ),

    EUI_FUNC( "sync", sync_begin_queues ),
    EUI_UINT8( "syncid", sync_id_val ),
    EUI_INT32_ARRAY( "tpos", target_position ),
    EUI_INT32_RO_ARRAY( "cpos", current_position ),

    //inbound movement buffer and 'add to queue' callback
    EUI_CUSTOM("inmv", motion_inbound),
    EUI_FUNC("stmv", execute_motion_queue),
    EUI_FUNC("clmv", clear_all_queue),

    // inbound led animation buffer and 'add to queue'
    EUI_CUSTOM("inlt", animation_inbound),

    // Event trigger callbacks
    EUI_FUNC("estop", emergency_stop_cb),
    EUI_FUNC("arm", start_mech_cb),
    EUI_FUNC("disarm", stop_mech_cb),
    EUI_FUNC("home", home_mech_cb),

    // UI requests a change of operating mode
    EUI_UINT8( "req_mode", mode_request ),

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
    EUI_TRACK( ui_variables );
    eui_setup_identifier( (char*)HAL_UUID, 12 );	//header byte is 96-bit, therefore 12-bytes

}

PUBLIC void
configuration_eui_callback( uint8_t link, eui_interface_t *interface, uint8_t message )
{
    // Provided the callbacks - use this to fire callbacks when a variable changes etc
    switch (message)
    {
        case EUI_CB_TRACKED:
        {
            // UI received a tracked message ID and has completed processing
            eui_header_t header = interface->packet.header;
            void *payload = interface->packet.data_in;
            uint8_t *name_rx = interface->packet.id_in;

            // See if the inbound packet name matches our intended variable
            if( strcmp( (char*)name_rx, "req_mode" ) == 0 )
            {

                // Fire an event to the supervisor to change mode
                switch( mode_request )
                {
                    case CONTROL_NONE:
                        // TODO allow UI to request a no-mode setting?
                        break;
                    case CONTROL_MANUAL:
                        eventPublish( EVENT_NEW( StateEvent, MODE_MANUAL ) );
                        break;
                    case CONTROL_EVENT:
                        eventPublish( EVENT_NEW( StateEvent, MODE_EVENT ) );
                        break;
                    case CONTROL_DEMO:
                        eventPublish( EVENT_NEW( StateEvent, MODE_DEMO ) );
                        break;
                    case CONTROL_TRACK:
                        eventPublish( EVENT_NEW( StateEvent, MODE_TRACK ) );
                        break;

                    default:
                        // Punish an incorrect attempt at mode changes with E-STOP
                        eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
                        break;
                }

            }

            if( strcmp( (char*)name_rx, "inmv" ) == 0 )
            {
                movement_generate_event();
            }

            if( strcmp( (char*)name_rx, "inlt" ) == 0 )
            {
                lighting_generate_event();
            }

            if( strcmp( (char*)name_rx, "tpos" ) == 0 )
            {
                tracked_position_event();
            }

            break;
        }

        case EUI_CB_UNTRACKED:
            // UI passed in an untracked message ID

            break;

        case EUI_CB_PARSE_FAIL:
            // Inbound message parsing failed, this callback help while debugging

            break;

        default:

            break;
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_report_error( char * error_string )
{
    // Send the text to the UI for display to user
    eui_message_t err_message = { .id = "err",
                                  .type = TYPE_CHAR,
                                  .size = strlen( error_string ),
                                { .data = error_string }       };

    eui_send_untracked( &err_message );
}

/* -------------------------------------------------------------------------- */

// System Statistics and Settings

PUBLIC void
config_set_cpu_load( uint8_t percent )
{
	sys_stats.cpu_load = percent;
}

PUBLIC void
config_set_cpu_clock(uint32_t clock)
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
config_set_main_state( uint8_t state )
{
	sys_states.supervisor = state;
	sys_states.motors = motion_servo[0].enabled || motion_servo[1].enabled || motion_servo[2].enabled;
	eui_send_tracked("super");
}

PUBLIC void
config_set_control_mode( uint8_t mode )
{
	sys_states.control_mode = mode;
    eui_send_tracked("super");
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
    current_position.x = x;
    current_position.y = y;
    current_position.z = z;
}

PUBLIC CartesianPoint_t
config_get_tracking_target()
{
	return target_position;
}

PUBLIC void
config_reset_tracking_target()
{
    target_position.x = 0;
    target_position.y = 0;
    target_position.z = 0;

    eui_send_tracked("tpos");   // tell the UI that the value has changed
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
    queue_data.movements = utilisation;
    eui_send_tracked("queue");
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
config_motor_target_angle( uint8_t servo, float angle )
{
	motion_servo[servo].target_angle = angle;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_led_status( uint8_t enabled )
{
    rgb_led_drive.enable = enabled;
}

PUBLIC void
config_set_led_values( uint16_t red, uint16_t green, uint16_t blue)
{
    rgb_led_drive.red = red;
    rgb_led_drive.green = green;
    rgb_led_drive.blue = blue;
}

PUBLIC void
config_set_led_queue_depth( uint8_t utilisation )
{
    queue_data.lighting = utilisation;
    eui_send_tracked("queue");
}

PUBLIC void
config_get_led_whitebalance( int16_t *red_offset, int16_t *green_offset, int16_t *blue_offset )
{
    *red_offset      = rgb_led_settings.balance_red;
    *green_offset    = rgb_led_settings.balance_green;
    *blue_offset     = rgb_led_settings.balance_blue;
}

PUBLIC void
config_get_led_bias( int16_t *offset )
{
    *offset      = rgb_led_settings.balance_total;
}

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void start_mech_cb( void )
{
	eventPublish( EVENT_NEW( StateEvent, MECHANISM_START ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void stop_mech_cb( void )
{
	eventPublish( EVENT_NEW( StateEvent, MECHANISM_STOP ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void emergency_stop_cb( void )
{
	eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void home_mech_cb( void )
{
	eventPublish( EVENT_NEW( StateEvent, MECHANISM_REHOME ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void movement_generate_event( void )
{
	   MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOVEMENT_REQUEST );

	   if(motion_request)
	   {
		   memcpy(&motion_request->move, &motion_inbound, sizeof(motion_inbound));
		   eventPublish( (StateEvent*)motion_request );
		   memset(&motion_inbound, 0, sizeof(motion_inbound));
	   }
}

PRIVATE void execute_motion_queue( void )
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_START ) );
}

PRIVATE void clear_all_queue(void)
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
    eventPublish( EVENT_NEW( StateEvent, LED_CLEAR_QUEUE ) );
}

PRIVATE void tracked_position_event( void )
{
    TrackedPositionRequestEvent *position_request = EVENT_NEW( TrackedPositionRequestEvent, TRACKED_TARGET_REQUEST );

    if(position_request)
    {
        memcpy(&position_request->target, &target_position, sizeof(target_position));
        eventPublish( (StateEvent*)position_request );
        memset(&target_position, 0, sizeof(target_position));
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void lighting_generate_event( void )
{
    LightingPlannerEvent *lighting_request = EVENT_NEW( LightingPlannerEvent, LED_QUEUE_ADD );

    if(lighting_request)
    {
        memcpy(&lighting_request->animation, &animation_inbound, sizeof(animation_inbound));
        eventPublish( (StateEvent*)lighting_request );
        memset(&animation_inbound, 0, sizeof(animation_inbound));
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void sync_begin_queues( void )
{
    BarrierSyncEvent *barrier_ev = EVENT_NEW( BarrierSyncEvent, START_QUEUE_SYNC );

    if(barrier_ev)
    {
        memcpy(&barrier_ev->id, &sync_id_val, sizeof(sync_id_val));
        eventPublish( (StateEvent*)barrier_ev );
        memset(&sync_id_val, 0, sizeof(sync_id_val));
    }
}

/* ----- End ---------------------------------------------------------------- */
