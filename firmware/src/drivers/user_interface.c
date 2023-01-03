#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "user_interface.h"
#include "user_interface_msgid.h"
#include "timer_ms.h"

#include "app_task_ids.h"
#include "app_tasks.h"

#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "app_version.h"
#include "event_subscribe.h"
#include "hal_uart.h"
#include "hal_uuid.h"

/* ----- Private Function Declaration --------------------------------------- */

PRIVATE void
user_interface_eui_callback( uint8_t link, eui_interface_t *interface, uint8_t message );

PRIVATE void user_interface_tx_put_external( uint8_t *c, uint16_t length );
PRIVATE void user_interface_eui_callback_external( uint8_t message );

PRIVATE void user_interface_tx_put_internal( uint8_t *c, uint16_t length );
PRIVATE void user_interface_eui_callback_internal( uint8_t message );

PRIVATE void user_interface_tx_put_module( uint8_t *c, uint16_t length );
PRIVATE void user_interface_eui_callback_module( uint8_t message );

/* ----- Private Function Declaration --------------------------------------- */

PRIVATE void start_mech_cb( void );
PRIVATE void stop_mech_cb( void );
PRIVATE void emergency_stop_cb( void );
PRIVATE void home_mech_cb( void );
PRIVATE void clear_all_queue( void );
PRIVATE void tracked_position_event( void );

PRIVATE void tracked_external_servo_request( void );

PRIVATE void rgb_manual_led_event( void );
PRIVATE void movement_generate_event( void );
PRIVATE void lighting_generate_event( void );
PRIVATE void sync_begin_queues( void );
PRIVATE void trigger_camera_capture( void );

/* ----- Defines ----------------------------------------------------------- */

char reset_cause[20]  = "No Reset Cause";
char assert_cause[32] = "No Assert";

KinematicsInfo_t mechanical_info;
BuildInfo_t      fw_info;

SystemData_t   system_stats;
SystemStates_t supervisor_states;
MotionData_t   motion_global;
FanData_t      fan_stats;

uint8_t mode_request = 0;

MotorData_t motion_servo[4];
float       external_servo_angle_target;

CartesianPoint_t current_position;    // global position of end effector in cartesian space
CartesianPoint_t target_position;

LedState_t rgb_led_drive;
LedState_t rgb_manual_control;

QueueDepths_t queue_data;
Movement_t    motion_inbound;
Fade_t        light_fade_inbound;

uint32_t camera_shutter_duration_ms = 0;

uint32_t attractor_species = 0;

eui_message_t ui_variables[] = {
    // Higher level system setup information
    EUI_CHAR_ARRAY_RO( MSGID_RESET_CAUSE, reset_cause ),
    EUI_CHAR_ARRAY_RO( MSGID_ASSERT_CAUSE, assert_cause ),
    EUI_CUSTOM( MSGID_FIRMWARE_INFO, fw_info ),
    //    EUI_CUSTOM_RO( MSGID_KINEMATICS, mechanical_info ),

    EUI_CUSTOM( MSGID_SYSTEM, system_stats ),
    EUI_CUSTOM( MSGID_SUPERVISOR, supervisor_states ),
    EUI_CUSTOM( MSGID_FAN, fan_stats ),
    EUI_CUSTOM_RO( MSGID_MOTION, motion_global ),
    EUI_CUSTOM_RO( MSGID_SERVO, motion_servo ),

    // UI requests a change of operating mode
    EUI_UINT8( MSGID_MODE_REQUEST, mode_request ),

    // Current and target positions in cartesian space
    EUI_CUSTOM( MSGID_POSITION_TARGET, target_position ),
    EUI_CUSTOM_RO( MSGID_POSITION_CURRENT, current_position ),

        EUI_FLOAT( MSGID_POSITION_EXPANSION, external_servo_angle_target),

    EUI_CUSTOM_RO( MSGID_LED, rgb_led_drive ),
    EUI_CUSTOM( MSGID_LED_MANUAL_REQUEST, rgb_manual_control ),

    // Movement/Lighting Queue Handling
    EUI_CUSTOM_RO( MSGID_QUEUE_INFO, queue_data ),
    EUI_FUNC( MSGID_QUEUE_CLEAR, clear_all_queue ),
    EUI_FUNC( MSGID_QUEUE_SYNC, sync_begin_queues ),
    EUI_CUSTOM( MSGID_QUEUE_ADD_FADE, light_fade_inbound ),
    EUI_CUSTOM( MSGID_QUEUE_ADD_MOVE, motion_inbound ),

    // Event trigger callbacks
    EUI_FUNC( MSGID_EMERGENCY_STOP, emergency_stop_cb ),
    EUI_FUNC( MSGID_ARM, start_mech_cb ),
    EUI_FUNC( MSGID_DISARM, stop_mech_cb ),
    EUI_FUNC( MSGID_HOME, home_mech_cb ),
    EUI_UINT32( MSGID_CAPTURE, camera_shutter_duration_ms ),

    EUI_UINT8( MSGID_ATTRACTOR_SPECIES, attractor_species ),

    // Configuration fields are stored in configuration.c, we access them directly via pointer manipulation
    { .id = MSGID_LED_CALIBRATION, .type = TYPE_CUSTOM, .size = sizeof(LedSettings_t), {.data = 0} },
    { .id = MSGID_POWER_CALIBRATION, .type = TYPE_CUSTOM, .size = sizeof(PowerCalibration_t), {.data = 0} },
    { .id = MSGID_FAN_CURVE, .type = TYPE_CUSTOM, .size = sizeof(FanCurve_t)*NUM_FAN_CURVE_POINTS, {.data = 0} },
    { .id = MSGID_CONFIG, .type = TYPE_CUSTOM, .size = sizeof(UserConfig_t), {.data = 0} }
};

#define HEARTBEAT_EXPECTED_MS 800	// Expect to see a heartbeat within this threshold
timer_ms_t last_heartbeat = 0;
uint8_t heartbeat_ok_count = 0;		// Running count of successful heartbeat messages

enum
{
    LINK_MODULE = 0,
    LINK_INTERNAL,
    LINK_EXTERNAL,
} EUI_LINK_NAMES;

eui_interface_t communication_interface[] = {
    EUI_INTERFACE_CB( &user_interface_tx_put_module, &user_interface_eui_callback_module ),
    EUI_INTERFACE_CB( &user_interface_tx_put_internal, &user_interface_eui_callback_internal ),
    EUI_INTERFACE_CB( &user_interface_tx_put_external, &user_interface_eui_callback_external ),
};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
user_interface_init( void )
{
    hal_uart_init( HAL_UART_PORT_MODULE );
    // TODO init other serial ports for UI use?

    EUI_LINK( communication_interface );
    EUI_TRACK( ui_variables );
    eui_setup_identifier( (char *)HAL_UUID, 12 );    // header byte is 96-bit, therefore 12-bytes

    // set build info to hardcoded values
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

    // Find MSGID_CONFIG and set the pointer to the configuration handler's struct data
    eui_message_t *tracked_config = find_tracked_object( MSGID_CONFIG );
    tracked_config->ptr.data = configuration_get_user_config_ptr();

    tracked_config = find_tracked_object( MSGID_FAN_CURVE );
    tracked_config->ptr.data = configuration_get_fan_curve_ptr();

    tracked_config = find_tracked_object( MSGID_LED_CALIBRATION );
    tracked_config->ptr.data = configuration_get_led_calibration_ptr();

    tracked_config = find_tracked_object( MSGID_POWER_CALIBRATION );
    tracked_config->ptr.data = configuration_get_power_calibration_ptr();

}

PUBLIC void
user_interface_handle_data( void )
{
    while( hal_uart_rx_data_available( HAL_UART_PORT_MODULE ) )
    {
        eui_parse( hal_uart_rx_get( HAL_UART_PORT_MODULE ), &communication_interface[LINK_MODULE] );
    }

    // TODO handle other communication link serial FIFO
}

PUBLIC bool
user_interface_connection_ok( void )
{
    // Check if the most recent heartbeats have timed out
    if( timer_ms_is_expired( &last_heartbeat ) )
    {
        heartbeat_ok_count = 0;
    }

    // Connection is considered OK if more than 3 heartbeats have arrived
    // within the threshold duration
    return ( heartbeat_ok_count > 3 );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
user_interface_tx_put_external( uint8_t *c, uint16_t length )
{
    hal_uart_write( HAL_UART_PORT_EXTERNAL, c, length );
}

PRIVATE void
user_interface_eui_callback_external( uint8_t message )
{
    user_interface_eui_callback( LINK_EXTERNAL, &communication_interface[LINK_EXTERNAL], message );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
user_interface_tx_put_internal( uint8_t *c, uint16_t length )
{
    hal_uart_write( HAL_UART_PORT_INTERNAL, c, length );
}

PRIVATE void
user_interface_eui_callback_internal( uint8_t message )
{
    user_interface_eui_callback( LINK_INTERNAL, &communication_interface[LINK_INTERNAL], message );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
user_interface_tx_put_module( uint8_t *c, uint16_t length )
{
    hal_uart_write( HAL_UART_PORT_MODULE, c, length );
}

PRIVATE void
user_interface_eui_callback_module( uint8_t message )
{
    user_interface_eui_callback( LINK_MODULE, &communication_interface[LINK_MODULE], message );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
user_interface_eui_callback( uint8_t link, eui_interface_t *interface, uint8_t message )
{
    // Provided the callbacks - use this to fire callbacks when a variable changes etc
    switch( message )
    {
        case EUI_CB_TRACKED: {
            // UI received a tracked message ID and has completed processing
            eui_header_t header  = interface->packet.header;
            void        *payload = interface->packet.data_in;
            uint8_t     *name_rx = interface->packet.id_in;

            bool has_payload = (header.data_len > 0);

            // See if the inbound packet name matches our intended variable
            if( strcmp( (char *)name_rx, MSGID_MODE_REQUEST ) == 0 )
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

            if( strcmp( (char *)name_rx, MSGID_QUEUE_ADD_MOVE ) == 0 && has_payload )
            {
                movement_generate_event();
            }

            if( strcmp( (char *)name_rx, MSGID_QUEUE_ADD_FADE ) == 0 && has_payload )
            {
                lighting_generate_event();
            }

            if( strcmp( (char *)name_rx, MSGID_POSITION_TARGET ) == 0 && has_payload )
            {
                tracked_position_event();
            }

            if( strcmp( (char *)name_rx, MSGID_POSITION_EXPANSION ) == 0 && header.data_len )
            {
                tracked_external_servo_request();
            }

            // Fire an event to refresh the LED if the UI sends a value in
            // This makes display updates when manually setting colors or calibration values nice and responsive
            if( strcmp( (char *)name_rx, MSGID_LED_MANUAL_REQUEST ) == 0 && has_payload
                || strcmp( (char *)name_rx, MSGID_LED_CALIBRATION ) == 0 && has_payload )
            {
                rgb_manual_led_event();
            }

            if( strcmp( (char *)name_rx, MSGID_CAPTURE ) == 0 && has_payload )
            {
                trigger_camera_capture();
            }

            if( strcmp( (char *)name_rx, MSGID_CONFIG ) == 0 && has_payload )
            {
                configuration_notify_config();
            }

            if( strcmp( (char *)name_rx, MSGID_FAN_CURVE ) == 0 && has_payload )
            {
                configuration_notify_fan_curve();
            }

            if( strcmp( (char*)name_rx, EUI_INTERNAL_HEARTBEAT ) == 0 )
            {
                if( !timer_ms_is_expired( &last_heartbeat ) )
                {
                    heartbeat_ok_count++;
                }

                timer_ms_start( &last_heartbeat, HEARTBEAT_EXPECTED_MS );
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
user_interface_set_reset_cause( const char *reset_description )
{
    memset( &reset_cause, 0, sizeof( reset_cause ) );
    strcpy( (char *)&reset_cause, reset_description );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_assert_cause( const char *assert_description )
{
    memset( &assert_cause, 0, sizeof( assert_cause ) );
    strcpy( (char *)&assert_cause, assert_description );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_report_error( char *error_string )
{
    // Send the text to the UI for display to user
    eui_message_t err_message = { .id   = MSGID_ERROR,
                                  .type = TYPE_CHAR,
                                  .size = strlen( error_string ),
                                  { .data = error_string } };

    eui_send_untracked( &err_message );
}

/* -------------------------------------------------------------------------- */

// System Statistics and Settings

PUBLIC void
user_interface_set_cpu_load( uint8_t percent )
{
    system_stats.cpu_load = percent;
}

PUBLIC void
user_interface_set_cpu_clock( uint32_t clock )
{
    system_stats.cpu_clock = clock / 1000000;    // convert to Mhz
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_sensors_enabled( bool enable )
{
    system_stats.sensors_enable = enable;
}

void user_interface_set_module_enable( bool enabled )
{
    system_stats.module_enable = enabled;
}

PUBLIC void
user_interface_set_input_voltage( float voltage )
{
    system_stats.input_voltage = voltage * 100;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_main_state( uint8_t state )
{
    supervisor_states.supervisor = state;
    supervisor_states.motors     = motion_servo[0].enabled || motion_servo[1].enabled || motion_servo[2].enabled;
    eui_send_tracked( MSGID_SUPERVISOR );
}

PUBLIC void
user_interface_set_control_mode( uint8_t mode )
{
    supervisor_states.control_mode = mode;
    eui_send_tracked( MSGID_SUPERVISOR );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_kinematics_mechanism_info( float shoulder_radius, float bicep_len, float forearm_len, float effector_radius )
{
    mechanical_info.shoulder_radius_micron = shoulder_radius;
    mechanical_info.bicep_length_micron    = bicep_len;
    mechanical_info.forearm_length_micron  = forearm_len;
    mechanical_info.effector_radius_micron = effector_radius;
}

PUBLIC void
user_interface_set_kinematics_limits( int32_t radius, int32_t zmin, int32_t zmax )
{
    mechanical_info.limit_radius = radius;
    mechanical_info.limit_z_min  = zmin;
    mechanical_info.limit_z_max  = zmax;
}

PUBLIC void
user_interface_set_kinematics_flips( int8_t x, int8_t y, int8_t z )
{
    mechanical_info.flip_x = x;
    mechanical_info.flip_y = y;
    mechanical_info.flip_z = z;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_fan_percentage( uint8_t percent )
{
    fan_stats.setpoint_percentage = percent;
}

PUBLIC void
user_interface_set_fan_rpm( uint16_t rpm )
{
    fan_stats.speed_rpm = rpm;
}

PUBLIC void
user_interface_set_fan_state( uint8_t state )
{
    fan_stats.state = state;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_temp_ambient( float temp )
{
    system_stats.temp_pcb_ambient = temp * 100;
}

PUBLIC void
user_interface_set_temp_regulator( float temp )
{
    system_stats.temp_pcb_regulator = temp * 100;
}

PUBLIC void
user_interface_set_temp_external( float temp )
{
    system_stats.temp_external_probe = temp * 100;
}

PUBLIC void
user_interface_set_temp_cpu( float temp )
{
    system_stats.temp_cpu = temp * 100;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_position( int32_t x, int32_t y, int32_t z )
{
    current_position.x = x;
    current_position.y = y;
    current_position.z = z;
}

PUBLIC void
user_interface_set_effector_speed( uint32_t microns_per_second )
{
    motion_global.effector_speed = microns_per_second;
}

PUBLIC CartesianPoint_t
user_interface_get_tracking_target()
{
    return target_position;
}

PUBLIC void
user_interface_reset_tracking_target()
{
    target_position.x = 0;
    target_position.y = 0;
    target_position.z = 0;

    eui_send_tracked( MSGID_POSITION_TARGET );    // tell the UI that the value has changed
}

PUBLIC void
user_interface_set_movement_data( uint32_t sync_offset, uint8_t move_type, uint8_t progress )
{
    motion_global.movement_identifier = sync_offset;
    motion_global.profile_type        = move_type;
    motion_global.move_progress       = progress;
}

PUBLIC void
user_interface_set_pathing_status( uint8_t status )
{
    motion_global.pathing_state = status;
}

PUBLIC void
user_interface_set_motion_state( uint8_t status )
{
    motion_global.motion_state = status;
}

PUBLIC void
user_interface_set_motion_queue_depth( uint8_t utilisation )
{
    queue_data.movements = utilisation;
    //    eui_send_tracked("queue");
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_motor_enable( uint8_t servo, bool enable )
{
    motion_servo[servo].enabled = enable;
}

PUBLIC void
user_interface_motor_state( uint8_t servo, uint8_t state )
{
    motion_servo[servo].state = state;
}

PUBLIC void
user_interface_motor_feedback( uint8_t servo, float percentage )
{
    motion_servo[servo].feedback = percentage * 10;
}

PUBLIC void
user_interface_motor_power( uint8_t servo, float watts )
{
    motion_servo[servo].power = watts;
}

PUBLIC void
user_interface_motor_speed( uint8_t servo, float degrees_per_second )
{
    motion_servo[servo].speed = degrees_per_second;
}

PUBLIC void
user_interface_motor_target_angle( uint8_t servo, float angle )
{
    motion_servo[servo].target_angle = angle;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_led_status( uint8_t enabled )
{
    rgb_led_drive.enable = enabled;
}

PUBLIC void
user_interface_set_led_values( uint16_t red, uint16_t green, uint16_t blue )
{
    rgb_led_drive.red   = red;
    rgb_led_drive.green = green;
    rgb_led_drive.blue  = blue;
}

PUBLIC void
user_interface_set_led_queue_depth( uint8_t utilisation )
{
    queue_data.lighting = utilisation;
    //    eui_send_tracked("queue");
}

PUBLIC void
user_interface_get_led_manual( float *r, float *g, float *b, uint8_t *en )
{
    *r  = rgb_manual_control.red;
    *g  = rgb_manual_control.green;
    *b  = rgb_manual_control.blue;
    *en = rgb_manual_control.enable;
}

PRIVATE void
rgb_manual_led_event()
{
    LightingManualEvent *colour_request = EVENT_NEW( LightingManualEvent, LED_MANUAL_SET );

    if( colour_request )
    {
        colour_request->colour.red   = rgb_manual_control.red;
        colour_request->colour.green = rgb_manual_control.green;
        colour_request->colour.blue  = rgb_manual_control.blue;
        colour_request->enabled      = rgb_manual_control.enable;

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

// Send an event to the supervisor requesting the start of the queues
// Supervisor will generate the epoch for timing reference, and fire events to the motion
// and LED planner tasks.

PRIVATE void sync_begin_queues( void )
{
    eventPublish( EVENT_NEW( StateEvent, QUEUE_SYNC_START ) );
}

PRIVATE void clear_all_queue( void )
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
    eventPublish( EVENT_NEW( StateEvent, LED_QUEUE_CLEAR ) );
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


/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_push_position( void )
{
    eui_send_tracked( MSGID_POSITION_CURRENT );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
user_interface_get_attractor_species( void )
{
    return attractor_species;
}


/* ----- End ---------------------------------------------------------------- */
