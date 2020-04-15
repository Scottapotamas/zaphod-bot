#ifndef APPLICATION_CONFIG_H
#define APPLICATION_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

/* ----- Defines ------------------------------------------------------------ */

#define APPLICATION_VERSION 0

//! Hardware Version - Currently a static fixed value.
#define HARDWARE_VERSION 1

//! Manufacturer name
#define MANUFACTURER_NAME "Scott Rapson"

/* -------------------------------------------------------------------------- */
/* --- Tick Timer Conversion Macros                                       --- */
/* -------------------------------------------------------------------------- */

#ifdef STM32F429xx
//! Application uses 1ms timer ticks on the ARM platform with SysTick
#define MS_PER_TICK 1U
#else
//! Application uses 10ms timer ticks on the PC
#define MS_PER_TICK 10UL
#endif

// Convert a ms time into timer ticks. Ensure that we round up when needed.
#define MS_TO_TICKS( _ms_ ) ( ( ( _ms_ ) + ( MS_PER_TICK - 1 ) ) / MS_PER_TICK )

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_CONFIG_H */
