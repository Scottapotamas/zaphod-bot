/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"
#include "eui_serial_transport.h"

typedef struct
{
	uint8_t cpu_load;
	float cpu_temp;
	float input_voltage;
	bool module_power;
} SystemData_t;

typedef struct
{
	uint8_t setpoint_percentage;
	uint16_t speed_rpm;
} FanData_t;

typedef struct
{
	//todo add all points of fan curve here, or just a single?
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
	int32_t x;
	int32_t y;
	int32_t z;
	uint8_t profile_type;
	uint8_t move_progress;
} KinematicsData_t;

typedef struct
{
	bool enabled;
	bool in_motion;
	bool error;
	float angle;
	float current;
	float power;
} MotorData_t;


SystemData_t sys_stats;
FanData_t fan_stats;
FanCurve_t fan_curve[5];
TempData_t temp_sensors;

KinematicsData_t motion_global;
MotorData_t motion_servo_1;
MotorData_t motion_servo_2;
MotorData_t motion_servo_3;


PRIVATE void toggleLed();

euiMessage_t ui_variables[] = {

    {.msgID = "tgl", .type = TYPE_CALLBACK, .size = sizeof(toggleLed), .payload = &toggleLed  },

    //type examples
    {.msgID = "sys", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(SystemData_t),  	.payload = &sys_stats       },
    {.msgID = "fan", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(FanData_t), 		.payload = &fan_curve      	},
    {.msgID = "curve", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(FanCurve_t), 	.payload = &example_uint32  },
    {.msgID = "temp", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(TempData_t),  	.payload = &temp_sensors    },

    {.msgID = "moStat",	.type = TYPE_CUSTOM_MARKER, .size = sizeof(KinematicsData_t),  	.payload = &motion_global },
    {.msgID = "mo1", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(MotorData_t),  	.payload = &motion_servo_1    },
    {.msgID = "mo2", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(MotorData_t),  	.payload = &motion_servo_2    },
    {.msgID = "mo3", 	.type = TYPE_CUSTOM_MARKER, .size = sizeof(MotorData_t),  	.payload = &motion_servo_3    },

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
