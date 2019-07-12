/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"

#include "app_task_communication.h"
#include "configuration.h"
#include "hal_uart.h"
#include "usbd_cdc_if.h"

#include "electricui.h"

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskCommunicationConstructor( AppTaskCommunication *me );

PRIVATE void AppTaskCommunication_initial( AppTaskCommunication *me, const StateEvent *e );

PRIVATE void AppTaskCommunication_tx_put_internal( uint8_t *c, uint16_t length );

PRIVATE void AppTaskCommunication_tx_put_external( uint8_t *c, uint16_t length );

PRIVATE void AppTaskCommunication_tx_put_module( uint8_t *c, uint16_t length );

PRIVATE void AppTaskCommunication_tx_put_usb( uint8_t *c, uint16_t length );

PRIVATE void AppTaskCommunication_rx_callback_uart( HalUartPort_t port, uint8_t c );

PRIVATE void AppTaskCommunication_rx_callback_cdc( uint8_t c );

PRIVATE STATE AppTaskCommunication_main( AppTaskCommunication *me, const StateEvent *e );

PRIVATE STATE AppTaskCommunication_electric_ui( AppTaskCommunication *me, const StateEvent *e );

enum {
    LINK_MODULE = 0,
    LINK_INTERNAL,
    LINK_EXTERNAL,
    LINK_USB
} EUI_LINK_NAMES;

eui_interface_t communication_interface[] = {
        EUI_INTERFACE(&AppTaskCommunication_tx_put_module),
        EUI_INTERFACE(&AppTaskCommunication_tx_put_internal),
        EUI_INTERFACE(&AppTaskCommunication_tx_put_external),
        EUI_INTERFACE(&AppTaskCommunication_tx_put_usb),
};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskCommunicationCreate(  AppTaskCommunication *me,
                         	 StateEvent        *eventQueueData[ ],
							 const uint8_t     eventQueueSize,
							 const CommunicationInstance_t instance )
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskCommunication) );

    // Initialise State Machine State
    AppTaskCommunicationConstructor( me );

    me->instance = instance;

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

        	switch (me->instance) {
				case INTERFACE_UART_MODULE:
		            hal_uart_init( HAL_UART_PORT_MODULE );
		            hal_uart_set_baudrate( HAL_UART_PORT_MODULE, MODULE_BAUD );
		        	hal_uart_set_rx_handler(HAL_UART_PORT_MODULE, AppTaskCommunication_rx_callback_uart );

					break;

				case INTERFACE_UART_INTERNAL:
		            hal_uart_init( HAL_UART_PORT_INTERNAL );
		            hal_uart_set_baudrate( HAL_UART_PORT_INTERNAL, EXTERNAL_BAUD );
		        	hal_uart_set_rx_handler(HAL_UART_PORT_INTERNAL, AppTaskCommunication_rx_callback_uart );

					break;

				case INTERFACE_UART_EXTERNAL:
		            hal_uart_init( HAL_UART_PORT_EXTERNAL );
		            hal_uart_set_baudrate( HAL_UART_PORT_EXTERNAL, INTERNAL_BAUD );
		        	hal_uart_set_rx_handler(HAL_UART_PORT_EXTERNAL, AppTaskCommunication_rx_callback_uart );

					break;

				case INTERFACE_USB_EXTERNAL:
					//todo init cdc here
					//todo setup callback to AppTaskCommunication_rx_callback_cdc

					break;
			}

			//eUI setup
			EUI_LINK( communication_interface );
        	configuration_electric_setup();	//get the configuration driver to setup access to variables

        	return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskCommunication_main;
}

PRIVATE void
AppTaskCommunication_tx_put_external( uint8_t *c, uint16_t length )
{
	hal_uart_write( HAL_UART_PORT_EXTERNAL, c, length );
}

PRIVATE void
AppTaskCommunication_tx_put_internal( uint8_t *c, uint16_t length)
{
	hal_uart_write( HAL_UART_PORT_INTERNAL, c, length );
}

PRIVATE void
AppTaskCommunication_tx_put_module( uint8_t *c, uint16_t length )
{
	hal_uart_write( HAL_UART_PORT_MODULE, c, length );
}


PRIVATE void
AppTaskCommunication_tx_put_usb( uint8_t *c, uint16_t length )
{
	CDC_Transmit_FS( c, length );
}

PRIVATE void
AppTaskCommunication_rx_callback_uart( HalUartPort_t port, uint8_t c )
{
	switch ( port )
	{
		case HAL_UART_PORT_MODULE:
			eui_parse(c, &communication_interface[LINK_MODULE]);
			break;

		case HAL_UART_PORT_EXTERNAL:
            eui_parse(c, &communication_interface[LINK_EXTERNAL]);
			break;

		case HAL_UART_PORT_INTERNAL:
            eui_parse(c, &communication_interface[LINK_INTERNAL]);
			break;

		default:
			break;
	}

}

PRIVATE void
AppTaskCommunication_rx_callback_cdc( uint8_t c )
{
    eui_parse(c, &communication_interface[LINK_USB]);

}
/* ----- End ---------------------------------------------------------------- */
