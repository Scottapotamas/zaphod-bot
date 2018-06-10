/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"

#include "app_task_communication.h"

#include "electricui.h"
#include <eui_serial_transport.h>

#include "status.h"
#include "hal_uart.h"

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskCommunicationConstructor( AppTaskCommunication *me );

PRIVATE void AppTaskCommunication_initial( AppTaskCommunication *me, const StateEvent *e );

PRIVATE void AppTaskCommunication_tx_put_internal( uint8_t c );

PRIVATE void AppTaskCommunication_tx_put_external( uint8_t c );

PRIVATE void AppTaskCommunication_tx_put_module( uint8_t c );

PRIVATE void AppTaskCommunication_rx_callback( HalUartPort_t port, uint8_t c );



PRIVATE STATE AppTaskCommunication_main( AppTaskCommunication *me, const StateEvent *e );

PRIVATE STATE AppTaskCommunication_electric_ui( AppTaskCommunication *me, const StateEvent *e );

eui_interface module_comms;

PRIVATE void toggleLed();

uint8_t   example_uint8   = 21;
uint16_t  example_uint16  = 321;
uint32_t  example_uint32  = 654321;
float     example_float   = 3.14159;

euiMessage_t dev_msg_store[] = {

    {.msgID = "tgl", .type = TYPE_CALLBACK, .size = sizeof(toggleLed),      .payload = &toggleLed       },

    //type examples
    {.msgID = "ui8", .type = TYPE_UINT8,  .size = sizeof(example_uint8),  .payload = &example_uint8       },
    {.msgID = "i16", .type = TYPE_UINT16, .size = sizeof(example_uint16), .payload = &example_uint16      },
    {.msgID = "i32", .type = TYPE_UINT32, .size = sizeof(example_uint32), .payload = &example_uint32      },
    {.msgID = "fPI", .type = TYPE_FLOAT,  .size = sizeof(example_float),  .payload = &example_float       },
};

void toggleLed()
{
  status_yellow_toggle();
}

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskCommunicationCreate(  AppTaskCommunication *me,
                         	 StateEvent        *eventQueueData[ ],
							 const uint8_t     eventQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskCommunication) );

    // Initialise State Machine State
    AppTaskCommunicationConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
                            eventQueueData,
                            eventQueueSize,
                            0,
                            0 );
}

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskCommunicationConstructor( AppTaskCommunication *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskCommunication_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskCommunication_initial( AppTaskCommunication *me,
                                        const StateEvent *e __attribute__((__unused__)) )
{

    STATE_INIT( &AppTaskCommunication_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskCommunication_main( AppTaskCommunication *me,
                                      const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        {

        	return 0;
        }

        case STATE_INIT_SIGNAL:
            STATE_INIT( &AppTaskCommunication_electric_ui );
            return 0;

    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskCommunication_electric_ui( AppTaskCommunication *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

        	//uart setup
            hal_uart_init( HAL_UART_PORT_MODULE );
            hal_uart_set_baudrate( HAL_UART_PORT_MODULE, 115200 );
        	hal_uart_set_rx_handler(HAL_UART_PORT_MODULE, AppTaskCommunication_rx_callback );

			//eUI setup
			module_comms.output_char_fnPtr = &AppTaskCommunication_tx_put_module;
			setup_dev_msg(dev_msg_store, ARR_ELEM(dev_msg_store));
			setup_identifier();

        	return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskCommunication_main;
}

PRIVATE void
AppTaskCommunication_tx_put_external( uint8_t c )
{
    hal_uart_put( HAL_UART_PORT_EXTERNAL, c );
}

PRIVATE void
AppTaskCommunication_tx_put_internal( uint8_t c )
{
    hal_uart_put( HAL_UART_PORT_INTERNAL, c );
}

PRIVATE void
AppTaskCommunication_tx_put_module( uint8_t c )
{
    hal_uart_put( HAL_UART_PORT_MODULE, c );
}

PRIVATE void
AppTaskCommunication_rx_callback( HalUartPort_t port, uint8_t c )
{
	switch ( port )
	{
		case HAL_UART_PORT_MODULE:
				parse_packet(c, &module_comms);
			break;

		case HAL_UART_PORT_EXTERNAL:

			break;

		case HAL_UART_PORT_INTERNAL:

			break;

		default:
			break;
	}

}

/* ----- End ---------------------------------------------------------------- */
