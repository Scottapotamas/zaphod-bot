#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- Local Includes ----------------------------------------------------- */

/* ----- Types -------------------------------------------------------------- */

typedef unsigned char uchar_t;    ///< Specifically unsigned char.

typedef float float32_t;    ///< Clarify that float is 32bit.

typedef union    ///< Allow mapping int16 to uint16
{                ///< without conversions
    int16_t  i16;
    uint16_t u16;
} Union16_t;

/* ----- Defines ------------------------------------------------------------ */

//#define EXPANSION_SERVO


//! \def PRIVATE
/// Makes it more clear that static functions/data are really private.
#define PRIVATE static

//! \def PUBLIC
/// Makes it more clear that the indicated data/function can be accessed
/// from outside the module.
#define PUBLIC

//! \def PROTECTED
/// Makes it more clear that the indicated data/function can be accessed
/// from outside the module but is supposed to be limited in scope like
/// only with 'friend' modules.
#define PROTECTED

/* -------------------------------------------------------------------------- */

/** Return true when the target processor is big endian */
#define IS_BIG_ENDIAN ( *(uint16_t *)"\0\xff" < 0x100 )

/* -------------------------------------------------------------------------- */

//! \def _BV(bit)
/// Basic bitvalue macro (suitable for up to 32 bits)
#ifndef _BV
  #define _BV(bit)         (1UL<<(bit))
#endif

/* -------------------------------------------------------------------------- */

//! \def DIM(_array_)
/// Calculate the dimension of an array
#define DIM(_array_)       (sizeof(_array_) / sizeof(_array_[0]))

/* -------------------------------------------------------------------------- */

/** Convert from 2-digit BCD to byte integer */
#define BCD2INT(_bcd_)    (((_bcd_)&0xf)+(10*(((_bcd_)>>4)&0xf)))

/** Convert from integer to 2-digit BCD  */
#define INT2BCD(_int_)    (((((_int_)/10)<<4)|((_int_)%10))&0xff)

/* -------------------------------------------------------------------------- */

/** Return average of two values */
#define AVG(X, Y)               (((X) + (Y)) / 2)

/** Return minimum of two values */
#ifndef MIN
	#define MIN(X, Y)               (((X) < (Y)) ? (X) : (Y))
#endif

/** Return maximum of two values */
#ifndef MAX
#define MAX(X, Y)               (((X) > (Y)) ? (X) : (Y))
#endif

/** Return the clamped value of a variable between a low and a high value */
#define CLAMP(X, LOW, HIGH)     (((X) > (HIGH)) ? (HIGH) : (((X) < (LOW)) ? (LOW) : (X)))

/** Convert various units into microns */
#define MM_TO_MICRONS( X ) ( X * 1000 )
#define CM_TO_MICRONS( X ) ( X * 10000 )

/** Convert microns into mm/cm */
#define MICRONS_TO_MM( X ) ( X / 1000 )
#define MICRONS_TO_CM( X ) ( X / 10000 )

#define IS_IN_DEADBAND( a, b, deadband ) ( abs( a - b ) <= deadband )

/* -------------------------------------------------------------------------- */

//! \def FORBID()
/// Disable interrupts
#ifdef STM32F429xx
#define FORBID() __disable_irq()
#else
#define FORBID()
#endif

//! \def PERMIT()
/// Enable interrupts
#ifdef STM32F429xx
#define PERMIT() __enable_irq()
#else
#define PERMIT()
#endif

//! \def CRITICAL_SECTION_START()
/// Save the current interrupt state and then disable interrupts to
/// enter a critical region of code.
#ifdef STM32F429xx
#define CRITICAL_SECTION_VAR() uint8_t cpuSR
#else
#define CRITICAL_SECTION_VAR()
#endif

//! \def CRITICAL_SECTION_START()
/// Save the current interrupt state and then disable interrupts to
/// enter a critical region of code.
#ifdef STM32F429xx
#define CRITICAL_SECTION_START()              \
        do {                                  \
          asm (                               \
          "MRS   R0, PRIMASK\n\t"             \
          "CPSID I\n\t"                       \
          "STRB R0, %[output]"                \
          : [output] "=m" (cpuSR) :: "r0");   \
        } while(0)
#else
#define CRITICAL_SECTION_START()
#endif

//! \def CRITICAL_SECTION_END()
/// Exit critical section of code and restore interrupt state
/// to what it was before \ref CRITICAL_SECTION_START.
#ifdef STM32F429xx
#define CRITICAL_SECTION_END()               \
        do{                                   \
          asm (                               \
          "ldrb r0, %[input]\n\t"             \
          "msr PRIMASK,r0;\n\t"               \
          ::[input] "m" (cpuSR) : "r0");      \
        } while(0)
#else
#define CRITICAL_SECTION_END()
#endif

//! \def ATOMIC()
/// Enclose a critical section of code that must be protected so that it
/// executes as a unit without interruption. Note that by using it like this
/// the compiler sees all _protected_code_ as a single line and this causes
/// some difficulty in looking at disassembled code in the debugger e.g.
/// when hunting for exceptions.
#define ATOMIC(_protected_code_) do                             \
                                 {                              \
                                     CRITICAL_SECTION_START();  \
                                     _protected_code_;          \
                                     CRITICAL_SECTION_END();    \
                                 } while(0)

/* ----- End ------------~--------------------------------------------------- */
#ifdef    __cplusplus
}
#endif
#endif
