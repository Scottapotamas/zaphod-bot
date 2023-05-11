#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "user_interface.h"
#include "user_interface_msgid.h"

#include "FreeRTOS.h"
#include "task.h"
#include <electricui.h>

#include "hal_uuid.h"
#include "hal_uart.h"
#include "version.h"
#include "signals.h"

//#include "configuration.h"
//#include "timer_ms.h"
//#include "app_task_ids.h"
//#include "app_tasks.h"
//#include "app_events.h"
//#include "app_signals.h"
//#include "app_times.h"
//#include "event_subscribe.h"

/* ----- Private Function Declaration --------------------------------------- */

PRIVATE void user_interface_sensors_callback(ObserverEvent_t event, EventData eData, void *context);


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

PRIVATE void tracked_external_servo_request( void );

PRIVATE void rgb_manual_led_event( void );
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
uint8_t demo_mode_request = 0;

MotorData_t motion_servo[4];
int32_t     external_servo_angle_target;

CartesianPoint_t current_position;    // global position of end effector in cartesian space
CartesianPoint_t target_position;

LedState_t rgb_led_drive;
LedState_t rgb_manual_control;

QueueDepths_t queue_data;
Movement_t    motion_inbound;
//Fade_t        light_fade_inbound;

uint32_t camera_shutter_duration_ms = 0;


eui_message_t ui_variables[] = {
    // Higher level system setup information
    EUI_CHAR_ARRAY_RO( MSGID_RESET_CAUSE, reset_cause ),
    EUI_CHAR_ARRAY_RO( MSGID_ASSERT_CAUSE, assert_cause ),
    EUI_CUSTOM( MSGID_FIRMWARE_INFO, fw_info ),
    //    EUI_CUSTOM_RO( MSGID_KINEMATICS, mechanical_info ),

    EUI_CUSTOM( MSGID_SYSTEM, system_stats ),
    EUI_CUSTOM( MSGID_SUPERVISOR, supervisor_states ),
//    EUI_CUSTOM( MSGID_FAN, fan_stats ),
    EUI_CUSTOM_RO( MSGID_MOTION, motion_global ),
    EUI_CUSTOM_RO( MSGID_SERVO, motion_servo ),

    // UI requests a change of operating mode
    EUI_UINT8( MSGID_MODE_REQUEST, mode_request ),

    // Current and target positions in cartesian space
    EUI_CUSTOM( MSGID_POSITION_TARGET, target_position ),
    EUI_CUSTOM_RO( MSGID_POSITION_CURRENT, current_position ),

    EUI_CUSTOM_RO( MSGID_LED, rgb_led_drive ),
    EUI_CUSTOM( MSGID_LED_MANUAL_REQUEST, rgb_manual_control ),

    // Movement/Lighting Queue Handling
    EUI_CUSTOM_RO( MSGID_QUEUE_INFO, queue_data ),
    EUI_FUNC( MSGID_QUEUE_CLEAR, clear_all_queue ),
    EUI_FUNC( MSGID_QUEUE_SYNC, sync_begin_queues ),
//    EUI_CUSTOM( MSGID_QUEUE_ADD_FADE, light_fade_inbound ),
    EUI_CUSTOM( MSGID_QUEUE_ADD_MOVE, motion_inbound ),

    // Event trigger callbacks
    EUI_FUNC( MSGID_EMERGENCY_STOP, emergency_stop_cb ),
    EUI_FUNC( MSGID_ARM, start_mech_cb ),
    EUI_FUNC( MSGID_DISARM, stop_mech_cb ),
    EUI_FUNC( MSGID_HOME, home_mech_cb ),

    EUI_CUSTOM( MSGID_POSITION_EXPANSION, external_servo_angle_target),
    EUI_UINT32( MSGID_CAPTURE, camera_shutter_duration_ms ),

//    EUI_UINT8( MSGID_DEMO_CONFIGURATION, demo_mode_configuration ),

    // Configuration fields are stored in configuration.c, we access them directly via pointer manipulation
//    { .id = MSGID_LED_CALIBRATION, .type = TYPE_CUSTOM, .size = sizeof(LedSettings_t), {.data = 0} },
//    { .id = MSGID_POWER_CALIBRATION, .type = TYPE_CUSTOM, .size = sizeof(PowerCalibration_t), {.data = 0} },
//    { .id = MSGID_FAN_CURVE, .type = TYPE_CUSTOM, .size = sizeof(FanCurve_t)*NUM_FAN_CURVE_POINTS, {.data = 0} },
//    { .id = MSGID_CONFIG, .type = TYPE_CUSTOM, .size = sizeof(UserConfig_t), {.data = 0} }
};

#define HEARTBEAT_EXPECTED_MS 800	// Expect to see a heartbeat within this threshold

// TODO update for FreeRTOS
uint32_t last_heartbeat = 0;
uint8_t heartbeat_ok_count = 0;		// Running count of successful heartbeat messages

enum
{
    LINK_MODULE = 0,
    LINK_INTERNAL,
    LINK_EXTERNAL,
} EUI_LINK_NAMES;

eui_interface_t communication_interface[] = {
    [LINK_MODULE]   = EUI_INTERFACE_CB( &user_interface_tx_put_module, &user_interface_eui_callback_module ),
    [LINK_INTERNAL] = EUI_INTERFACE_CB( &user_interface_tx_put_internal, &user_interface_eui_callback_internal ),
    [LINK_EXTERNAL] = EUI_INTERFACE_CB( &user_interface_tx_put_external, &user_interface_eui_callback_external ),
};

PRIVATE Observer telemetry_observer = { 0 };
PRIVATE Subject event_subject = { 0 };

PRIVATE MovementRequestFn handle_requested_move;    // Pass UI Movement event requests to this callback for handling
PRIVATE PositionRequestFn handle_requested_position; // Pass cartesian position requests to this callback for handling

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
user_interface_init( void )
{
    // We need to do a full reset of USART clocks/peripherals on boot, as the DMA setup seems
    // to cling onto error flags across firmware flashing - making debugging hard
    hal_uart_global_deinit();

    hal_uart_init( HAL_UART_PORT_MODULE, 500000 );
    hal_uart_init( HAL_UART_PORT_INTERNAL, 500000 );
    hal_uart_init( HAL_UART_PORT_EXTERNAL, 500000 );

    // Electric UI Setup
    eui_setup_tracked( ui_variables, EUI_ARR_ELEM(ui_variables) );
    eui_setup_interfaces( communication_interface, EUI_ARR_ELEM(communication_interface) );
    eui_setup_identifier( (char *)HAL_UUID, 12 );    // header byte is 96-bit, therefore 12-bytes

    // Setup the UI request subject
    subject_init(&event_subject);

    // Subscribe to the sensor events
    observer_init( &telemetry_observer, user_interface_sensors_callback, NULL );

    observer_subscribe( &telemetry_observer, SENSOR_FAN_SPEED );
    observer_subscribe( &telemetry_observer, SENSOR_VOLTAGE_INPUT );
    observer_subscribe( &telemetry_observer, SENSOR_TEMPERATURE_PCB );
    observer_subscribe( &telemetry_observer, SENSOR_TEMPERATURE_REGULATOR );
    observer_subscribe( &telemetry_observer, SENSOR_TEMPERATURE_EXTERNAL );
    observer_subscribe( &telemetry_observer, SENSOR_TEMPERATURE_MICRO );

    observer_subscribe( &telemetry_observer, SENSOR_SERVO_CURRENT );
    observer_subscribe( &telemetry_observer, SENSOR_SERVO_HLFB );
    observer_subscribe( &telemetry_observer, SERVO_STATE );
    observer_subscribe( &telemetry_observer, SERVO_POSITION );
    observer_subscribe( &telemetry_observer, SERVO_SPEED );

//    observer_subscribe( &telemetry_observer, SERVO_SPEED );

    observer_subscribe( &telemetry_observer, OVERWATCH_STATE_UPDATE );
    observer_subscribe( &telemetry_observer, OVERWATCH_MODE_UPDATE );


    // Set build info to hardcoded values
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
//    eui_message_t *tracked_config = find_tracked_object( MSGID_CONFIG );
//    tracked_config->ptr.data = configuration_get_user_config_ptr();
//
//    tracked_config = find_tracked_object( MSGID_FAN_CURVE );
//    tracked_config->ptr.data = configuration_get_fan_curve_ptr();
//
//    tracked_config = find_tracked_object( MSGID_LED_CALIBRATION );
//    tracked_config->ptr.data = configuration_get_led_calibration_ptr();
//
//    tracked_config = find_tracked_object( MSGID_POWER_CALIBRATION );
//    tracked_config->ptr.data = configuration_get_power_calibration_ptr();

}

#define PARSE_CHUNK_SIZE 32

uint8_t buffer[PARSE_CHUNK_SIZE] = { 0 };
uint32_t bytes_available = 0;

PUBLIC void
user_interface_task( void *arg )
{

    for(;;)
    {
        // Read data from the UART into the buffer
        // Iterate over the received bytes and pass them to the eUI parser

        bytes_available = hal_uart_read( HAL_UART_PORT_MODULE, buffer, PARSE_CHUNK_SIZE);
        for( uint32_t i = 0; i < bytes_available; i++ )
        {
            eui_parse( buffer[i], &communication_interface[LINK_MODULE] );
        }

        bytes_available = hal_uart_read( HAL_UART_PORT_INTERNAL, buffer, PARSE_CHUNK_SIZE);
        for( uint32_t i = 0; i < bytes_available; i++ )
        {
            eui_parse( buffer[i], &communication_interface[LINK_INTERNAL] );
        }

        bytes_available = hal_uart_read( LINK_EXTERNAL, buffer, PARSE_CHUNK_SIZE);
        for( uint32_t i = 0; i < bytes_available; i++ )
        {
            eui_parse( buffer[i], &communication_interface[LINK_EXTERNAL] );
        }

        vTaskDelay(1);
    }
}

PUBLIC Observer * user_interface_get_observer( void )
{
    return &telemetry_observer;
}

PUBLIC Subject * user_interface_get_request_subject( void )
{
    return &event_subject;
}

PUBLIC void user_interface_attach_motion_request_cb( MovementRequestFn callback )
{
    handle_requested_move = callback;
}

PUBLIC void user_interface_attach_position_request_cb( PositionRequestFn callback )
{
    handle_requested_position = callback;
}


PRIVATE void user_interface_sensors_callback(ObserverEvent_t event, EventData eData, void *context)
{
    switch( event )
    {
        case SENSOR_FAN_SPEED:
            fan_stats.speed_rpm = eData.data.f32;
            break;

        case SENSOR_VOLTAGE_INPUT:
            system_stats.input_voltage = eData.data.f32 * 100;
            break;

        case SENSOR_TEMPERATURE_PCB:
            system_stats.temp_pcb_ambient = eData.data.f32 * 100;
            break;

        case SENSOR_TEMPERATURE_REGULATOR:
            system_stats.temp_pcb_regulator = eData.data.f32 * 100;
            break;

        case SENSOR_TEMPERATURE_EXTERNAL:
            system_stats.temp_external_probe = eData.data.f32 * 100;
            break;

        case SENSOR_TEMPERATURE_MICRO:
            system_stats.temp_cpu = eData.data.f32 * 100;
            break;

        // TODO: telemetry task needs to handle all other sensor values

        case SENSOR_SERVO_CURRENT:
            motion_servo[eData.index].power = eData.data.f32;

            break;

        case SENSOR_SERVO_HLFB:
            motion_servo[eData.index].feedback = eData.data.f32 * 10.0f;
            break;

        case SERVO_STATE:
            motion_servo[eData.index].state = eData.data.u32;

            if( eData.index != 0 )
            {
                asm("NOP");
            }

            break;

        case SERVO_POSITION:
            motion_servo[eData.index].target_angle = eData.data.u32;
            break;

        case OVERWATCH_STATE_UPDATE:
            supervisor_states.supervisor = eData.data.u32;
//            eui_send_tracked( MSGID_SUPERVISOR );       // TODO: don't send now, do a debounced send somewhere more automatically?
            break;

        case OVERWATCH_MODE_UPDATE:
            supervisor_states.control_mode = eData.data.u32;
//            eui_send_tracked( MSGID_SUPERVISOR );       // TODO also consider for automatic debounced output
            break;

//            supervisor_states.motors     = motion_servo[0].enabled || motion_servo[1].enabled || motion_servo[2].enabled;



        default:
            // Why did we recieve a signal that we didn't subscribe to?
//            ASSERT(false);
            break;
    }
}

PUBLIC bool
user_interface_connection_ok( void )
{
    // TODO update this for FreeRTOS
    // Check if the most recent heartbeats have timed out
//    if( timer_ms_is_expired( &last_heartbeat ) )
//    {
//        heartbeat_ok_count = 0;
//    }

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
                EventData modeValue = { .data.u32 = mode_request };
                subject_notify( &event_subject, FLAG_MODE_REQUEST, modeValue );
            }

            if( strcmp( (char *)name_rx, MSGID_DEMO_CONFIGURATION ) == 0 && has_payload )
            {
                EventData modeValue = { .data.u32 = demo_mode_request };
                subject_notify( &event_subject, FLAG_MODE_REQUEST, modeValue );
            }

            if( strcmp( (char *)name_rx, MSGID_QUEUE_ADD_MOVE ) == 0 && has_payload )
            {
                if( handle_requested_move )
                {
                    handle_requested_move( &motion_inbound );
                }
            }

            if( strcmp( (char *)name_rx, MSGID_QUEUE_ADD_FADE ) == 0 && has_payload )
            {
                lighting_generate_event();
            }

            if( strcmp( (char *)name_rx, MSGID_POSITION_TARGET ) == 0 && has_payload )
            {
                if( handle_requested_position )
                {
                    handle_requested_position( &target_position );
                }
            }

            if( strcmp( (char *)name_rx, MSGID_POSITION_EXPANSION ) == 0 && header.data_len )
            {
                tracked_external_servo_request();
            }

            // Fire an event to refresh the LED if the UI sends a value in
            // This makes display updates when manually setting colors or calibration values nice and responsive
            if(    ( strcmp( (char *)name_rx, MSGID_LED_MANUAL_REQUEST ) == 0 && has_payload )
                || ( strcmp( (char *)name_rx, MSGID_LED_CALIBRATION ) == 0 && has_payload ) )
            {
                rgb_manual_led_event();
            }



            if( strcmp( (char *)name_rx, MSGID_CAPTURE ) == 0 && has_payload )
            {
                trigger_camera_capture();
            }

            if( strcmp( (char *)name_rx, MSGID_CONFIG ) == 0 && has_payload )
            {
//                configuration_notify_config();
            }

            if( strcmp( (char *)name_rx, MSGID_FAN_CURVE ) == 0 && has_payload )
            {
//                configuration_notify_fan_curve();
            }

            if( strcmp( (char*)name_rx, EUI_INTERNAL_HEARTBEAT ) == 0 )
            {
                // TODO update heartbeat monitor for FreeRTOS
//                if( !timer_ms_is_expired( &last_heartbeat ) )
//                {
//                    heartbeat_ok_count++;
//                }

//                timer_ms_start( &last_heartbeat, HEARTBEAT_EXPECTED_MS );
            }
            break;
        }

        case EUI_CB_UNTRACKED:
            // UI passed in an untracked message ID

            break;

        case EUI_CB_PARSE_FAIL:
            // Inbound message parsing failed, this callback can help while debugging
            user_interface_report_error("eui pf");
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

/* -------------------------------------------------------------------------- */

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
user_interface_set_fan_state( uint8_t state )
{
    fan_stats.state = state;
}

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_position( int32_t x, int32_t y, int32_t z )
{
//    current_position.x = x;
//    current_position.y = y;
//    current_position.z = z;
}

PUBLIC void
user_interface_set_effector_speed( uint32_t microns_per_second )
{
    motion_global.effector_speed = microns_per_second;
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

PRIVATE void
rgb_manual_led_event()
{
//    LightingManualEvent *colour_request = EVENT_NEW( LightingManualEvent, LED_MANUAL_SET );
//
//    if( colour_request )
//    {
//        colour_request->colour.red   = rgb_manual_control.red;
//        colour_request->colour.green = rgb_manual_control.green;
//        colour_request->colour.blue  = rgb_manual_control.blue;
//        colour_request->enabled      = rgb_manual_control.enable;
//
//        eventPublish( (StateEvent *)colour_request );
//    }
}

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void start_mech_cb( void )
{
    EventData temp = { 0 };
    subject_notify( &event_subject, FLAG_ARM, temp );
}

/* -------------------------------------------------------------------------- */

PRIVATE void stop_mech_cb( void )
{
    EventData temp = { 0 };
    subject_notify( &event_subject, FLAG_DISARM, temp );
}

/* -------------------------------------------------------------------------- */

PRIVATE void emergency_stop_cb( void )
{
    EventData temp = { 0 };
    subject_notify( &event_subject, FLAG_ESTOP, temp );

    // TODO add the concept of 'e-stop sources'
}

/* -------------------------------------------------------------------------- */

PRIVATE void home_mech_cb( void )
{
    EventData temp = { 0 };
    subject_notify( &event_subject, FLAG_REHOME, temp );
}

/* -------------------------------------------------------------------------- */

PRIVATE void tracked_external_servo_request( void )
{
//    ExpansionServoRequestEvent *angle_request = EVENT_NEW( ExpansionServoRequestEvent, TRACKED_EXTERNAL_SERVO_REQUEST );
//
//    if( angle_request )
//    {
//        memcpy( &angle_request->target, &external_servo_angle_target, sizeof( external_servo_angle_target ) );
//        eventPublish( (StateEvent *)angle_request );
//        memset( &external_servo_angle_target, 0, sizeof( external_servo_angle_target ) );
//    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void lighting_generate_event( void )
{
//    LightingPlannerEvent *lighting_request = EVENT_NEW( LightingPlannerEvent, LED_QUEUE_ADD );
//
//    if( lighting_request )
//    {
//        memcpy( &lighting_request->animation, &light_fade_inbound, sizeof( light_fade_inbound ) );
//        eventPublish( (StateEvent *)lighting_request );
//        memset( &light_fade_inbound, 0, sizeof( light_fade_inbound ) );
//    }
}

/* -------------------------------------------------------------------------- */

// Send an event to the supervisor requesting the start of the queues
// Supervisor will generate the epoch for timing reference, and fire events to the motion
// and LED planner tasks.

PRIVATE void sync_begin_queues( void )
{
//    eventPublish( EVENT_NEW( StateEvent, QUEUE_SYNC_START ) );
}

PRIVATE void clear_all_queue( void )
{
//    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
//    eventPublish( EVENT_NEW( StateEvent, LED_QUEUE_CLEAR ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void
trigger_camera_capture( void )
{
//    CameraShutterEvent *trigger = EVENT_NEW( CameraShutterEvent, CAMERA_CAPTURE );
//
//    if( trigger )
//    {
//        memcpy( &trigger->exposure_time, &camera_shutter_duration_ms, sizeof( camera_shutter_duration_ms ) );
//        eventPublish( (StateEvent *)trigger );
//        memset( &camera_shutter_duration_ms, 0, sizeof( camera_shutter_duration_ms ) );
//    }
}


/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_push_position( void )
{
    eui_send_tracked( MSGID_POSITION_CURRENT );
}

/* -------------------------------------------------------------------------- */



/* ----- End ---------------------------------------------------------------- */
