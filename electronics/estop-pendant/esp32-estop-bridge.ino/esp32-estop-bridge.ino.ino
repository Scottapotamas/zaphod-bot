/* ESP32 E-STOP Pendant Bridge
 * 
 * Sits between a host PC serial connection (i.e. USB to the ESP32's UART IC) and the
 * Delta's isolated external IO via 9-pin LEMO connector.
 * 
 * Serial traffic is passed straight through as a transparent bridge, but sniffs the data
 * to catch the delta's supervisor state which is used for LED control.
 * 
 * Button press events generate an ESTOP packet which is sent to the delta.
 *  At this time, the delta can only assume this is from the user-interface.
 */

#include "electricui.h"

// Serial Connections
#define BAUDRATE 500000
#define UART_RX 16
#define UART_TX 17

// Status LED Dimming
#define LED_PIN 13
#define LED_PWM_CHANNEL 0
#define LED_PWM_HZ 5000
#define LED_PWM_BITS 10
#define LED_PWM_MAX_CNT (1<<LED_PWM_BITS)

// ESTOP Button
#define BUTTON_PIN 12
#define BUTTON_DEBOUNCE_MS 40

enum {
  BUTTON_UP = 0,
  BUTTON_WAIT,  // crude debouncing
  BUTTON_DOWN,
} BUTTON_STATES;

uint8_t button_state = BUTTON_UP;
uint32_t button_timer = 0;

uint16_t status_duty = 0;

// -----------------------------------------------------

enum {
  EUI_DELTA = 0,
  EUI_PC,
  EUI_NUM_INTERFACES,
} EUI_INTERFACES;

// As we're only sniffing - writing in the callback is commented out
void eui_write_delta( uint8_t *data, uint16_t len );
void eui_write_pc( uint8_t *data, uint16_t len );

void sniff_delta_callback( uint8_t message );
void sniff_pc_callback( uint8_t message );

// Maintain two different interfaces for each side of the bridge
eui_interface_t sniff_interfaces[EUI_NUM_INTERFACES] = {
  EUI_INTERFACE_CB( &eui_write_delta, &sniff_delta_callback ),
  EUI_INTERFACE_CB( &eui_write_pc,    &sniff_pc_callback ),
};

// -----------------------------------------------------

#define MSGID_SUPERVISOR "ss"
#define MSGID_EMERGENCY_STOP "E"

typedef enum
{
    SUPERVISOR_NONE = 0,
    SUPERVISOR_IDLE,
    SUPERVISOR_ARMING,
    SUPERVISOR_ERROR,
    SUPERVISOR_ARMED,
    SUPERVISOR_DISARMING,
    SUPERVISOR_INVALID,
} SupervisorStates_t;

typedef struct
{
    uint8_t supervisor;
    uint8_t motors;
    uint8_t control_mode;
} SystemStates_t;

SystemStates_t delta_supervisor_state;

// Track the variables we're interested in
eui_message_t sniff_variables[] = 
{
  EUI_CUSTOM( MSGID_SUPERVISOR,  delta_supervisor_state ),
};

// -----------------------------------------------------

void init_serial( void ) 
{
  // USB connection to host PC
  Serial.begin( BAUDRATE );

  // Hardware connection to delta via LEMO cable
  Serial2.begin( BAUDRATE, SERIAL_8N1, UART_RX, UART_TX );
  
  EUI_LINK(  sniff_interfaces );
  EUI_TRACK( sniff_variables  );
}

// Proxy serial traffic between the host PC and delta control board
// Sniff for some specific eUI packets
void process_serial( void )
{
  // Delta to PC relay
  while( Serial2.available() )
  {
    uint8_t inbound_byte = Serial2.read();
    Serial.write( inbound_byte );

    eui_parse( inbound_byte, &sniff_interfaces[EUI_DELTA] );
  }

  // PC to Delta relay
  while( Serial.available() )
  {
    uint8_t inbound_byte = Serial.read();
    Serial2.write( inbound_byte );

    eui_parse( inbound_byte, &sniff_interfaces[EUI_PC] );
  }
}

void eui_write_pc( uint8_t *data, uint16_t len )
{
//  Serial.write( data, len );
}

void eui_write_delta( uint8_t *data, uint16_t len )
{
  Serial2.write( data, len );
}

void sniff_delta_callback( uint8_t message )
{
  switch(message)
  {
    case EUI_CB_TRACKED:
    {
      eui_header_t header   = sniff_interfaces[EUI_DELTA].packet.header;
      uint8_t      *name_rx = sniff_interfaces[EUI_DELTA].packet.id_in;
      void         *payload = sniff_interfaces[EUI_DELTA].packet.data_in;

      // Sniff for the packet of interest
      if( strcmp( (char*)name_rx, MSGID_SUPERVISOR ) == 0 )
      {
        // TODO: something with this info?
      }
    }
    break;

    case EUI_CB_UNTRACKED:
    break;
  }
}

void sniff_pc_callback( uint8_t message )
{
  switch(message)
  {
    case EUI_CB_TRACKED:
    {
      eui_header_t header   = sniff_interfaces[EUI_PC].packet.header;
      uint8_t      *name_rx = sniff_interfaces[EUI_PC].packet.id_in;
      void         *payload = sniff_interfaces[EUI_PC].packet.data_in;

      // Sniff for the packet of interest
      if( strcmp( (char*)name_rx, "h" ) == 0 )
      {
        // TODO: something with this info?
      }
    }
    break;

    case EUI_CB_UNTRACKED:
    break;
  }
}

// -----------------------------------------------------

void init_led( void )
{
  ledcSetup( LED_PWM_CHANNEL, LED_PWM_HZ, LED_PWM_BITS );
  ledcAttachPin( LED_PIN, LED_PWM_CHANNEL );
}

void process_led( void )
{
  
  switch( delta_supervisor_state.supervisor )
  {    
    case SUPERVISOR_NONE:
    case SUPERVISOR_INVALID:
      status_duty = 0;
    break;

    case SUPERVISOR_IDLE:
      // slow, dim heartbeat to show that it's alive
      status_duty = 20;
    break;
    
    case SUPERVISOR_ARMING:
    case SUPERVISOR_DISARMING:
      // Quicker, brighter heartbeat to make it obvious that things are happening
      status_duty = 1000;
    break;

    case SUPERVISOR_ARMED:
      // Continuously on at some brightness level?
      status_duty = 500;
    break;

    case SUPERVISOR_ERROR:
      // Short strobe/flashes to draw attention to fault?
      status_duty = 250;
    break;
  }
  
  ledcWrite( LED_PWM_CHANNEL, status_duty );
}

// -----------------------------------------------------

void init_button( void )
{
  pinMode( BUTTON_PIN, INPUT_PULLUP );
}

void process_button( void )
{
  // Due to pull-up, goes LOW when pressed
  bool button_up = digitalRead( BUTTON_PIN );
 
  // Identify if the button has been pressed
  switch( button_state )
  {
    case BUTTON_UP:
      if( !button_up )
      {
        button_timer = millis();
        button_state = BUTTON_WAIT;
      }
    break;
    
    case BUTTON_WAIT:
      if( millis() - button_timer >= BUTTON_DEBOUNCE_MS )
      {
          button_state = BUTTON_DOWN;
      }
    
      if( button_up )
      {
        button_state = BUTTON_UP;
      }
    break;
    
    case BUTTON_DOWN:
      // Crude hack to only fire event once per press
      if( button_timer )
      {
        button_timer = 0;
          
        // Send a ESTOP callback packet to the delta
        uint8_t placeholder = 0x08;
        eui_message_t estop_msg = { .id = MSGID_EMERGENCY_STOP,
                                    .type = TYPE_CALLBACK,
                                    .size = 0,
                                  { .data = &placeholder }       
                                  };
    
        eui_send_untracked_on( &estop_msg, &sniff_interfaces[EUI_DELTA] );
      }

      // Wait for button to be released
      if( button_up )
      {
        button_state = BUTTON_UP;
      }
    break;
  }
  
}

// -----------------------------------------------------

void setup() 
{
  init_serial();
  init_led();
  init_button();
}

void loop( void )
{
  process_serial();
  process_led();
  process_button();

  vTaskDelay(1);
}
