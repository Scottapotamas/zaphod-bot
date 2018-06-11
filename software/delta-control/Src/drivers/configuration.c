/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"
#include "eui_serial_transport.h"

/*
| Group        	| Component          	| Task                                      	| Implemented 	|
|--------------	|--------------------	|-------------------------------------------	|:-----------:	|
| Fan Control  	| Speed %            	| Show current duty cycle                   	|             	|
|              	| Speed RPM          	| Show current RPM                          	|             	|
|              	| Fan Curve          	| Allow edits of temp/speed curve           	|             	|
| Temperatures 	| PCB Ambient        	| In open-air part of PCB                   	|             	|
|              	| PCB 12V Reg        	| Underneath 12V regulator                  	|             	|
|              	| External Probe     	| Flying probe (near 75V PSU)               	|             	|
| Motion       	| Status             	| Idle, homing, transit, line, spline, etc  	|             	|
|              	| Cartesian Pos      	| (X, Y, Z) in 20.05mm format               	|             	|
|              	| Angular Pos        	| Shaft angles in 34.5° format              	|             	|
| Status       	| System State       	| Overall hardware status                   	|             	|
|              	| LED 0              	| Indication matching onboard leds          	|             	|
|              	| LED 1              	|                                           	|             	|
|              	| LED 2              	|                                           	|             	|
|              	| Button 0           	| State of button presses (accept UI press) 	|             	|
|              	| Button 1           	|                                           	|             	|
|              	| CPU Load %         	| Utilisation Stat                          	|             	|
|              	| Micro Core Temp    	| Internal temperature sensor               	|             	|
|              	| Add-in card state  	| If add-in card is powered or not          	|             	|
| Motor X      	| State              	| Passive, Homing, Idle, Motion, Error      	|             	|
|              	| Feedback Signal    	| -100% to +100% value for velocity/torque  	|             	|
|              	| Current Position   	| Internal tracked current position         	|             	|
|              	| Target Position    	| Servo driver target                       	|             	|
|              	| Power              	| Consumption in Watts                      	|             	|
|              	| Current            	| Draw in 1.25A format                      	|             	|
|              	| Current Fault Flag 	| Indication from sense IC of OC event      	|             	|
*/

typedef struct {

} KinematicsOverview;

PRIVATE void toggleLed();

uint8_t   example_uint8   = 21;
uint16_t  example_uint16  = 321;
uint32_t  example_uint32  = 654321;
float     example_float   = 3.14159;

euiMessage_t ui_variables[] = {

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

PUBLIC void
configuration_init( void )
{
	//perform any setup here if needed
}

PUBLIC void
configuration_set_defaults( void )
{
	//set variables back to their hardcoded defaults

}

PUBLIC void
configuration_load( void )
{
	//load settings from memory
	//todo implement flash storage mechanism

}

PUBLIC void
configuration_save( void )
{
	//save settings to memory

}

PUBLIC void
configuration_electric_setup( void )
{
	setup_dev_msg(ui_variables, ARR_ELEM(ui_variables));
}

/* ----- Private Functions -------------------------------------------------- */




/* ----- End ---------------------------------------------------------------- */
