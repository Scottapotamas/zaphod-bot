#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "demo_move.h"

#include "app_times.h"
#include "event_subscribe.h"
#include "app_events.h"
#include "app_signals.h"

PRIVATE void publish_motion_cube( uint8_t step );
bool cube_wanted = false;
uint8_t current_step = 0;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
sequence_event_handle( void )
{
	if(cube_wanted && current_step < 16)
	{
		publish_motion_cube( current_step );
		current_step++;
	}
	else
	{
		current_step = 0;
		cube_wanted = false;
	}
}

PUBLIC void
cube_request( void )
{
	cube_wanted = true;
}

/* --------------------------------------------------------------------- */


PRIVATE void publish_motion_cube( uint8_t step )
{

	   MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_ADD_REQUEST );

	   if(motev)
	   {
		   motev->move.type = _LINE;
		   motev->move.ref = _POS_ABSOLUTE;
		   motev->move.duration = 500;
		   motev->move.num_pts = 2;

		   switch(step)
		   {
		   case 0:
			   //transit to starting position
			   motev->move.type = _POINT_TRANSIT;
			   motev->move.duration = 1500;

			   motev->move.points[0].x = 0;
			   motev->move.points[0].y = 0;
			   motev->move.points[0].z = 0;
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 1:
			   //b1
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 2:
			   //b2
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 3:
			   //b3
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 4:
			   //b4
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 5:
			   //v1
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 6:
			   //gap
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 7:
			   //v2
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 8:
			   //gap
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 9:
			   //v3
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 10:
			   //gap
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(10);
			   break;

		   case 11:
			   //v4
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(10);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 12:
			   //t4
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 13:
			   //t1
			   motev->move.points[0].x = MM_TO_MICRONS(20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 14:
			   //t2
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(-20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   break;

		   case 15:
			   //t3
			   motev->move.points[0].x = MM_TO_MICRONS(-20);
			   motev->move.points[0].y = MM_TO_MICRONS(-20);
			   motev->move.points[0].z = MM_TO_MICRONS(50);
			   motev->move.points[1].x = MM_TO_MICRONS(20);
			   motev->move.points[1].y = MM_TO_MICRONS(-20);
			   motev->move.points[1].z = MM_TO_MICRONS(50);
			   cube_wanted = false;

			   break;

		   default:
			   //do nothing, its a relative move!
			   motev->move.ref = _POS_RELATIVE;

			   motev->move.points[0].x = MM_TO_MICRONS(0);
			   motev->move.points[0].y = MM_TO_MICRONS(0);
			   motev->move.points[0].z = MM_TO_MICRONS(0);
			   motev->move.points[1].x = MM_TO_MICRONS(0);
			   motev->move.points[1].y = MM_TO_MICRONS(0);
			   motev->move.points[1].z = MM_TO_MICRONS(0);
			   cube_wanted = false;
			   break;
		   }

		   eventPublish( (StateEvent*)motev );

	   }
}


/* ----- End ---------------------------------------------------------------- */
