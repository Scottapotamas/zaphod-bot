/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "electricui.h"
#include "eui_serial_transport.h"

#include "hal_uuid.h"
#include "app_version.h"

typedef struct
{
	//microcontroller info
	uint8_t cpu_load;	//percentage
	float cpu_clock;	//speed in Mhz
	float cpu_temp;		//temp in degrees
	bool sensors_enable;	//if ADC sampling and conversions are active

	float input_voltage;	//voltage
	bool module_enable;		//add-in card powered

} SystemData_t;

typedef struct
{
	char * build_branch;
	char * build_info;
	char * build_date;
	char * build_time;
} BuildInfo_t;

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
	bool in_motion;
	bool error;
	float angle;
	float current;
	float power;
} MotorData_t;

SystemData_t 	sys_stats;
BuildInfo_t		fw_info;
FanData_t 		fan_stats;
FanCurve_t 		fan_curve[5];
TempData_t 		temp_sensors;

MotionData_t 	motion_global;
MotorData_t 	motion_servo_1;
MotorData_t 	motion_servo_2;
MotorData_t 	motion_servo_3;

PRIVATE void toggleLed();

euiMessage_t ui_variables[] = {

    {.msgID = "tgl", .type = TYPE_CALLBACK, .size = sizeof(toggleLed), .payload = &toggleLed  },

    //type examples
    {.msgID = "sys", 	.type = TYPE_CUSTOM, .size = sizeof(SystemData_t),  .payload = &sys_stats       },
    {.msgID = "fwb", 	.type = TYPE_CUSTOM, .size = sizeof(BuildInfo_t), 	.payload = &fw_info      	},
    {.msgID = "fan", 	.type = TYPE_CUSTOM, .size = sizeof(FanData_t), 	.payload = &fan_stats      	},
    {.msgID = "curve", 	.type = TYPE_CUSTOM, .size = sizeof(FanCurve_t), 	.payload = &fan_curve  		},
    {.msgID = "temp", 	.type = TYPE_CUSTOM, .size = sizeof(TempData_t),  	.payload = &temp_sensors 	},

    {.msgID = "moStat",	.type = TYPE_CUSTOM, .size = sizeof(MotionData_t), 	.payload = &motion_global 	},
    {.msgID = "mo1", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo_1 	},
    {.msgID = "mo2", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo_2 	},
    {.msgID = "mo3", 	.type = TYPE_CUSTOM, .size = sizeof(MotorData_t),  	.payload = &motion_servo_3 	},

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
	fw_info.build_branch 	= ProgramBuildBranch;
	fw_info.build_info 		= ProgramBuildInfo;
	fw_info.build_date 		= ProgramBuildDate;
	fw_info.build_time 		= ProgramBuildTime;

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
	setup_identifier( HAL_UUID, 12 );	//header byte is 96-bit, therefore 12-bytes
	setup_dev_msg(ui_variables, ARR_ELEM(ui_variables));
}

/* ----- Private Functions -------------------------------------------------- */




/* ----- End ---------------------------------------------------------------- */
