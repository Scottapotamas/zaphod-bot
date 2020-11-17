#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"

#include "app_task_ids.h"
#include "app_tasks.h"

#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "app_version.h"
#include "buzzer.h"
#include "event_subscribe.h"
#include "hal_flashmem.h"
#include "hal_uuid.h"

typedef struct
{
    bool sensors_enable;    //if ADC sampling and conversions are active
    bool module_enable;     //add-in card powered

    //microcontroller info
    uint8_t cpu_load;     //percentage
    uint8_t cpu_clock;    //speed in Mhz

    float input_voltage;    //voltage
} SystemData_t;

typedef struct
{
    uint8_t supervisor;
    uint8_t motors;
    uint8_t control_mode;
} SystemStates_t;

typedef struct
{
    char build_branch[12];
    char build_info[12];
    char build_date[12];
    char build_time[12];
    char build_type[12];
    char build_name[12];
} BuildInfo_t;

typedef struct
{
    uint8_t  id;    // index of the task (pseudo priority)
    uint8_t  ready;
    uint8_t  queue_used;
    uint8_t  queue_max;
    uint32_t waiting_max;
    uint32_t burst_max;
    char     name[12];    // human readable taskname set during app_tasks setup
} Task_Info_t;

typedef struct
{
    // Dimensions used in the IK/FK calculations
    uint32_t shoulder_radius_micron;
    uint32_t bicep_length_micron;
    uint32_t forearm_length_micron;
    uint32_t effector_radius_micron;

    // Limits in cartesian space
    int32_t limit_radius;
    int32_t limit_z_min;
    int32_t limit_z_max;

    // Flags if an axis is inverted
    int8_t flip_x;
    int8_t flip_y;
    int8_t flip_z;

} KinematicsInfo_t;

typedef struct
{
    uint16_t speed_rpm;
    uint8_t  setpoint_percentage;
    uint8_t  state;
} FanData_t;

uint8_t fan_manual_setpoint = 0;
uint8_t fan_manual_enable   = 0;

typedef struct
{
    float pcb_ambient;
    float pcb_regulator;
    float external_probe;
    float cpu_temp;
} TempData_t;

typedef struct
{
    //pathing engine state idle, running, etc
    uint8_t pathing_state;
    //motion handler information
    uint8_t motion_state;
    //information about the current move being executed
    uint8_t  profile_type;
    uint8_t  move_progress;
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
    int16_t feedback;
    float   target_angle;
    float   power;
} MotorData_t;

typedef struct
{
    int16_t voltage;
    int16_t current_servo_1;
    int16_t current_servo_2;
    int16_t current_servo_3;
    int16_t current_servo_4;
} PowerCalibration_t;

typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint8_t  enable;
} LedState_t;

typedef struct
{
    float   hue;
    float   saturation;
    float   lightness;
    uint8_t enable;
} LedControl_t;

typedef struct
{
    int16_t balance_red;
    int16_t balance_green;
    int16_t balance_blue;
    int16_t balance_total;
} LedSettings_t;

SystemData_t     sys_stats;
BuildInfo_t      fw_info;
Task_Info_t      task_info[TASK_MAX] = { 0 };
KinematicsInfo_t mechanical_info;

FanData_t  fan_stats;
FanCurve_t fan_curve[] = {
    { .temperature = 0, .percentage = 20 },
    { .temperature = 20, .percentage = 20 },
    { .temperature = 35, .percentage = 45 },
    { .temperature = 45, .percentage = 90 },
    { .temperature = 60, .percentage = 100 },
};

TempData_t temp_sensors;

SystemStates_t sys_states;
QueueDepths_t  queue_data;

MotionData_t motion_global;
#ifdef EXPANSION_SERVO
MotorData_t motion_servo[4];
float external_servo_angle_target;
#else
MotorData_t  motion_servo[3];

#endif

PowerCalibration_t power_trims;

Movement_t       motion_inbound;
CartesianPoint_t current_position;    //global position of end effector in cartesian space
CartesianPoint_t target_position;

LedState_t    rgb_led_drive;
LedControl_t  rgb_manual_control;
LedSettings_t rgb_led_settings;
Fade_t        light_fade_inbound;

float z_rotation = 0;

char device_nickname[16] = "Zaphod Beeblebot";
char reset_cause[20]     = "No Reset Cause";

PRIVATE void start_mech_cb( void );
PRIVATE void stop_mech_cb( void );
PRIVATE void emergency_stop_cb( void );
PRIVATE void home_mech_cb( void );
PRIVATE void execute_motion_queue( void );
PRIVATE void clear_all_queue( void );
PRIVATE void tracked_position_event( void );
#ifdef EXPANSION_SERVO
PRIVATE void tracked_external_servo_request( void );
#endif

PRIVATE void rgb_manual_led_event( void );
PRIVATE void movement_generate_event( void );
PRIVATE void lighting_generate_event( void );
PRIVATE void sync_begin_queues( void );
PRIVATE void trigger_camera_capture( void );

PRIVATE void configuration_wipe( void );
uint16_t     sync_id_val  = 0;
uint8_t      mode_request = 0;

uint32_t camera_shutter_duration_ms = 0;

eui_message_t ui_variables[] = {
    // Higher level system setup information
    EUI_CHAR_ARRAY_RO( "name", device_nickname ),
    EUI_CHAR_ARRAY_RO( "reset_type", reset_cause ),
    EUI_CUSTOM( "sys", sys_stats ),
    EUI_CUSTOM( "super", sys_states ),
    EUI_CUSTOM( "fwb", fw_info ),
    EUI_CUSTOM( "tasks", task_info ),
    EUI_CUSTOM_RO( "kinematics", mechanical_info ),

    // Temperature and cooling system
    EUI_CUSTOM( "fan", fan_stats ),
    EUI_CUSTOM( "curve", fan_curve ),
    EUI_CUSTOM( "temp", temp_sensors ),

    EUI_UINT8( "fan_man_speed", fan_manual_setpoint ),
    EUI_UINT8( "fan_manual_en", fan_manual_enable ),

    // motion related information
    EUI_CUSTOM_RO( "queue", queue_data ),

    EUI_CUSTOM_RO( "moStat", motion_global ),
    EUI_CUSTOM_RO( "servo", motion_servo ),

    EUI_CUSTOM( "pwr_cal", power_trims ),
    EUI_CUSTOM_RO( "rgb", rgb_led_drive ),
    EUI_CUSTOM( "hsv", rgb_manual_control ),
    EUI_CUSTOM( "ledset", rgb_led_settings ),

    //inbound movement buffer and 'add to queue' callback
    EUI_CUSTOM( "inlt", light_fade_inbound ),
    EUI_CUSTOM( "inmv", motion_inbound ),

    EUI_FUNC( "stmv", execute_motion_queue ),
    EUI_FUNC( "clmv", clear_all_queue ),
    EUI_FUNC( "sync", sync_begin_queues ),
    EUI_UINT16( "syncid", sync_id_val ),

    EUI_INT32_ARRAY( "tpos", target_position ),
    EUI_INT32_ARRAY_RO( "cpos", current_position ),

#ifdef EXPANSION_SERVO
    EUI_FLOAT( "exp_ang", external_servo_angle_target),
#endif

    // Event trigger callbacks
    EUI_FUNC( "estop", emergency_stop_cb ),
    EUI_FUNC( "arm", start_mech_cb ),
    EUI_FUNC( "disarm", stop_mech_cb ),
    EUI_FUNC( "home", home_mech_cb ),

    // UI requests a change of operating mode
    EUI_UINT8( "req_mode", mode_request ),

    EUI_FLOAT( "rotZ", z_rotation ),
    EUI_UINT32( "capture", camera_shutter_duration_ms ),

    EUI_FUNC( "save", configuration_save ),
    EUI_FUNC( "load", configuration_load ),
    EUI_FUNC( "wipe", configuration_wipe ),
};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
configuration_init( void )
{
    //perform any setup here if needed
    configuration_set_defaults();

    // Load settings from flash memory
    //    configuration_load();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_set_defaults( void )
{
    //set build info to hardcoded values
    memset( &fw_info.build_branch, 0, sizeof( fw_info.build_branch ) );
    memset( &fw_info.build_info, 0, sizeof( fw_info.build_info ) );
    memset( &fw_info.build_date, 0, sizeof( fw_info.build_date ) );
    memset( &fw_info.build_time, 0, sizeof( fw_info.build_time ) );
    memset( &fw_info.build_type, 0, sizeof( fw_info.build_type ) );
    memset( &fw_info.build_name, 0, sizeof( fw_info.build_name ) );

    strcpy( (char *)&fw_info.build_branch, ProgramBuildBranch );
    strcpy( (char *)&fw_info.build_info, ProgramBuildInfo );
    strcpy( (char *)&fw_info.build_date, ProgramBuildDate );
    strcpy( (char *)&fw_info.build_time, ProgramBuildTime );
    strcpy( (char *)&fw_info.build_type, ProgramBuildType );
    strcpy( (char *)&fw_info.build_name, ProgramName );
}

/* -------------------------------------------------------------------------- */

#define PERSIST_ID_CAL_POWER 1
#define PERSIST_ID_CAL_LED   2
#define PERSIST_ID_FAN_CURVE 3

PUBLIC void
configuration_load( void )
{
    // Load the data from non-volatile storage
    hal_flashmem_retrieve( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_retrieve( PERSIST_ID_CAL_LED, &rgb_led_settings, sizeof( LedSettings_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_save( void )
{
    //save settings to memory
    hal_flashmem_store( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_store( PERSIST_ID_CAL_LED, &rgb_led_settings, sizeof( LedSettings_t ) );

    buzzer_sound( 2, 4000, 50 );
}

PRIVATE void configuration_wipe( void )
{
    hal_flashmem_wipe_and_prepare();
    buzzer_sound( 1, 1000, 100 );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_electric_setup( void )
{
    EUI_TRACK( ui_variables );
    eui_setup_identifier( (char *)HAL_UUID, 12 );    //header byte is 96-bit, therefore 12-bytes
}

PUBLIC void
configuration_eui_callback( uint8_t link, eui_interface_t *interface, uint8_t message )
{
    // Provided the callbacks - use this to fire callbacks when a variable changes etc
    switch( message )
    {
        case EUI_CB_TRACKED: {
            // UI received a tracked message ID and has completed processing
            eui_header_t header  = interface->packet.header;
            void *       payload = interface->packet.data_in;
            uint8_t *    name_rx = interface->packet.id_in;

            // See if the inbound packet name matches our intended variable
            if( strcmp( (char *)name_rx, "req_mode" ) == 0 )
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

            if( strcmp( (char *)name_rx, "inmv" ) == 0 && header.data_len )
            {
                movement_generate_event();
            }

            if( strcmp( (char *)name_rx, "inlt" ) == 0 && header.data_len )
            {
                lighting_generate_event();
            }

            if( strcmp( (char *)name_rx, "tpos" ) == 0 && header.data_len )
            {
                tracked_position_event();
            }

#ifdef EXPANSION_SERVO
            if( strcmp( (char *)name_rx, "exp_ang" ) == 0 && header.data_len )
            {
                tracked_external_servo_request();
            }
#endif
            
            if( ( strcmp( (char *)name_rx, "hsv" ) == 0 || strcmp( (char *)name_rx, "ledset" ) == 0 )
                && header.data_len )
            {
                rgb_manual_led_event();
            }

            if( strcmp( (char *)name_rx, "capture" ) == 0 && header.data_len )
            {
                trigger_camera_capture();
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
config_set_reset_cause( const char *reset_description )
{
    memset( &reset_cause, 0, sizeof( reset_cause ) );
    strcpy( (char *)&reset_cause, reset_description );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_report_error( char *error_string )
{
    // Send the text to the UI for display to user
    eui_message_t err_message = { .id   = "err",
                                  .type = TYPE_CHAR,
                                  .size = strlen( error_string ),
                                  { .data = error_string } };

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
config_set_cpu_clock( uint32_t clock )
{
    sys_stats.cpu_clock = clock / 1000000;    //convert to Mhz
}

PUBLIC void
config_update_task_statistics( void )
{
    for( uint8_t id = ( TASK_MAX - 1 ); id > 0; id-- )
    {
        StateTask *t = app_task_by_id( id );
        if( t )
        {
            task_info[id].id          = t->id;
            task_info[id].ready       = t->ready;
            task_info[id].queue_used  = t->eventQueue.used;
            task_info[id].queue_max   = t->eventQueue.max;
            task_info[id].waiting_max = t->waiting_max;
            task_info[id].burst_max   = t->burst_max;

            memset( &task_info[id].name, 0, sizeof( task_info[0].name ) );
            strcpy( (char *)&task_info[id].name, t->name );
        }
    }
    //app_task_clear_statistics();
}


/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_sensors_enabled( bool enable )
{
    sys_stats.sensors_enable = enable;
}

void config_set_module_enable( bool enabled )
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
    sys_states.motors     = motion_servo[0].enabled || motion_servo[1].enabled || motion_servo[2].enabled;
    eui_send_tracked( "super" );
}

PUBLIC void
config_set_control_mode( uint8_t mode )
{
    sys_states.control_mode = mode;
    eui_send_tracked( "super" );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_kinematics_mechanism_info( float shoulder_radius, float bicep_len, float forearm_len, float effector_radius )
{
    mechanical_info.shoulder_radius_micron = shoulder_radius;
    mechanical_info.bicep_length_micron    = bicep_len;
    mechanical_info.forearm_length_micron  = forearm_len;
    mechanical_info.effector_radius_micron = effector_radius;
}

PUBLIC void
config_set_kinematics_limits( int32_t radius, int32_t zmin, int32_t zmax )
{
    mechanical_info.limit_radius = radius;
    mechanical_info.limit_z_min  = zmin;
    mechanical_info.limit_z_max  = zmax;
}

PUBLIC void
config_set_kinematics_flips( int8_t x, int8_t y, int8_t z )
{
    mechanical_info.flip_x = x;
    mechanical_info.flip_y = y;
    mechanical_info.flip_z = z;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
config_get_fan_manual_control()
{
    return ( fan_manual_enable > 0 );
}

PUBLIC uint8_t
config_get_fan_target( void )
{
    return fan_manual_setpoint;
}

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

PUBLIC FanCurve_t *
config_get_fan_curve_ptr( void )
{
    if( DIM( fan_curve ) == NUM_FAN_CURVE_POINTS )
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

PUBLIC void
config_set_temp_cpu( float temp )
{
    temp_sensors.cpu_temp = temp;
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

    eui_send_tracked( "tpos" );    // tell the UI that the value has changed
}

PUBLIC void
config_set_movement_data( uint8_t move_id, uint8_t move_type, uint8_t progress )
{
    motion_global.movement_identifier = move_id;
    motion_global.profile_type        = move_type;
    motion_global.move_progress       = progress;
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
    //    eui_send_tracked("queue");
}

PUBLIC float
config_get_rotation_z()
{
    float angle = CLAMP( z_rotation, 0.0f, 360.0f );
    return angle;
}

PUBLIC int16_t config_get_voltage_trim_mV( void )
{
    return power_trims.voltage;
}

PUBLIC int16_t config_get_servo_trim_mA( uint8_t servo )
{
    switch( servo )
    {
        case 0:    // Servo 1
            return power_trims.current_servo_1;
        case 1:    // Servo 2
            return power_trims.current_servo_2;
        case 2:    // Servo 3
            return power_trims.current_servo_3;
        case 3:    // Servo 4
            return power_trims.current_servo_4;
        default:
            return 0;
    }
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
config_motor_feedback( uint8_t servo, float percentage )
{
    motion_servo[servo].feedback = percentage * 10;
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
config_set_led_values( uint16_t red, uint16_t green, uint16_t blue )
{
    rgb_led_drive.red   = red;
    rgb_led_drive.green = green;
    rgb_led_drive.blue  = blue;
}

PUBLIC void
config_set_led_queue_depth( uint8_t utilisation )
{
    queue_data.lighting = utilisation;
    //    eui_send_tracked("queue");
}

PUBLIC void
config_get_led_whitebalance( int16_t *red_offset, int16_t *green_offset, int16_t *blue_offset )
{
    *red_offset   = rgb_led_settings.balance_red;
    *green_offset = rgb_led_settings.balance_green;
    *blue_offset  = rgb_led_settings.balance_blue;
}

PUBLIC void
config_get_led_bias( int16_t *offset )
{
    *offset = rgb_led_settings.balance_total;
}

PUBLIC void
config_get_led_manual( float *h, float *s, float *l, uint8_t *en )
{
    *h  = rgb_manual_control.hue;
    *s  = rgb_manual_control.saturation;
    *l  = rgb_manual_control.lightness;
    *en = rgb_manual_control.enable;
}

PRIVATE void
rgb_manual_led_event()
{
    LightingManualEvent *colour_request = EVENT_NEW( LightingManualEvent, LED_MANUAL_SET );

    if( colour_request )
    {
        colour_request->colour.hue        = rgb_manual_control.hue;
        colour_request->colour.saturation = rgb_manual_control.saturation;
        colour_request->colour.intensity  = rgb_manual_control.lightness;
        colour_request->enabled           = rgb_manual_control.enable;

        eventPublish( (StateEvent *)colour_request );
    }
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

    if( motion_request )
    {
        memcpy( &motion_request->move, &motion_inbound, sizeof( motion_inbound ) );
        eventPublish( (StateEvent *)motion_request );
        memset( &motion_inbound, 0, sizeof( motion_inbound ) );
    }
}

PRIVATE void execute_motion_queue( void )
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_START ) );
}

PRIVATE void clear_all_queue( void )
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
    eventPublish( EVENT_NEW( StateEvent, LED_CLEAR_QUEUE ) );
}

PRIVATE void tracked_position_event( void )
{
    TrackedPositionRequestEvent *position_request = EVENT_NEW( TrackedPositionRequestEvent, TRACKED_TARGET_REQUEST );

    if( position_request )
    {
        memcpy( &position_request->target, &target_position, sizeof( target_position ) );
        eventPublish( (StateEvent *)position_request );
        memset( &target_position, 0, sizeof( target_position ) );
    }
}

#ifdef EXPANSION_SERVO
PRIVATE void tracked_external_servo_request( void )
{
    ExpansionServoRequestEvent *angle_request = EVENT_NEW( ExpansionServoRequestEvent, TRACKED_EXTERNAL_SERVO_REQUEST );

    if( angle_request )
    {
        memcpy( &angle_request->target, &external_servo_angle_target, sizeof( external_servo_angle_target ) );
        eventPublish( (StateEvent *)angle_request );
        memset( &external_servo_angle_target, 0, sizeof( external_servo_angle_target ) );
    }
}
#endif

/* -------------------------------------------------------------------------- */

PRIVATE void lighting_generate_event( void )
{
    LightingPlannerEvent *lighting_request = EVENT_NEW( LightingPlannerEvent, LED_QUEUE_ADD );

    if( lighting_request )
    {
        memcpy( &lighting_request->animation, &light_fade_inbound, sizeof( light_fade_inbound ) );
        eventPublish( (StateEvent *)lighting_request );
        memset( &light_fade_inbound, 0, sizeof( light_fade_inbound ) );
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void sync_begin_queues( void )
{
    BarrierSyncEvent *barrier_ev = EVENT_NEW( BarrierSyncEvent, START_QUEUE_SYNC );

    if( barrier_ev )
    {
        memcpy( &barrier_ev->id, &sync_id_val, sizeof( sync_id_val ) );
        eventPublish( (StateEvent *)barrier_ev );
        memset( &sync_id_val, 0, sizeof( sync_id_val ) );
    }
}

/* -------------------------------------------------------------------------- */
PRIVATE void
trigger_camera_capture( void )
{
    CameraShutterEvent *trigger = EVENT_NEW( CameraShutterEvent, CAMERA_CAPTURE );

    if( trigger )
    {
        memcpy( &trigger->exposure_time, &camera_shutter_duration_ms, sizeof( camera_shutter_duration_ms ) );
        eventPublish( (StateEvent *)trigger );
        memset( &camera_shutter_duration_ms, 0, sizeof( camera_shutter_duration_ms ) );
    }
}

/* ----- End ---------------------------------------------------------------- */
