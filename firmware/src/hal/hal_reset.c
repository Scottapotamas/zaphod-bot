/* ----- Local Includes ----------------------------------------------------- */

#include "hal_reset.h"
#include "stm32f4xx_ll_rcc.h"

#include "hal_delay.h"
#include "hal_watchdog.h"

/* Assert printout requirements */
#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------------------------- */

typedef struct
{
    uint32_t magic_flag;
    uint32_t length;
    char     message[32];
} AssertMessageDatagram_t;

#define MAGIC_FLAG_ASSERT 0xB000B000
#define MAGIC_FLAG_ASSERT_CLEARED 0xFAFAFAFA

// Hardcoded location in SRAM to cache assert message across a reboot
PRIVATE volatile AssertMessageDatagram_t *const assert_datagram = (AssertMessageDatagram_t *)0x2000FFF0;

PRIVATE AssertMessageDatagram_t cached_assert_datagram = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC HalPowerResetCause_t
hal_reset_cause( void )
{
    HalPowerResetCause_t cause = RESET_CAUSE_UNKNOWN;

    if( LL_RCC_IsActiveFlag_BORRST() )
    {
        cause = RESET_CAUSE_BOR;
    }
    else if( LL_RCC_IsActiveFlag_SFTRST() )
    {
        cause = RESET_CAUSE_SW;
    }
    else if( LL_RCC_IsActiveFlag_PORRST() )
    {
        cause = RESET_CAUSE_POR;
    }
    else if( LL_RCC_IsActiveFlag_PINRST() )
    {
        cause = RESET_CAUSE_PIN;
    }
    else if( LL_RCC_IsActiveFlag_LPWRRST() )
    {
        cause = RESET_CAUSE_LPWR;
    }
    else if( LL_RCC_IsActiveFlag_IWDGRST() )
    {
        cause = RESET_CAUSE_IWDG;
    }
    else if( LL_RCC_IsActiveFlag_WWDGRST() )
    {
        cause = RESET_CAUSE_WWDG;
    }

    LL_RCC_ClearResetFlags();

    return cause;
}

/* -------------------------------------------------------------------------- */

PUBLIC const char *
hal_reset_cause_description( HalPowerResetCause_t cause )
{
    switch( cause )
    {
        case RESET_CAUSE_BOR:
            return "Brown Out";
        case RESET_CAUSE_PIN:
            return "Reset Pin";
        case RESET_CAUSE_POR:
            return "Power On";
        case RESET_CAUSE_SW:
            return "Software Reset";
        case RESET_CAUSE_IWDG:
            return "Independent Watchdog";
        case RESET_CAUSE_WWDG:
            return "Window Watchdog";
        case RESET_CAUSE_LPWR:
            return "Low Power";
        default:
            return "Unknown Reset Cause";
    }
}

/* -------------------------------------------------------------------------- */

// Trigger a reboot of the microcontroller core
PUBLIC void
hal_reset_software( void )
{
    hal_delay_ms( 10 );    // allow some time for serial buffers to flush etc
    hal_watchdog_refresh();
    NVIC_SystemReset();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_reset_assert_cache( const char *file,
                        unsigned    line,
                        const char *fmt,
                        va_list     args )
{
    assert_datagram->magic_flag = MAGIC_FLAG_ASSERT;
    assert_datagram->length     = 0;

    memset( assert_datagram->message, 0, sizeof( assert_datagram->message ) );

    // Format the printf part if user's assert includes a message
    if( fmt && ( strlen( fmt ) > 0 ) )
    {
        assert_datagram->length = vsnprintf( assert_datagram->message, sizeof( assert_datagram->message ), fmt, args );
    }
    else    // print the filename and line number
    {
        // the *file arg is normally the whole path, we only care about the filename
        char *filename = strrchr( file, '/');
        assert_datagram->length = snprintf( assert_datagram->message, sizeof( assert_datagram->message ), "%s:%d", filename, line );
    }
}

PUBLIC char *
hal_reset_assert_description( void )
{
    if( assert_datagram->magic_flag != MAGIC_FLAG_ASSERT )
    {
        return "_";
    }

    // Cache the data from SRAM magic location
    memcpy( &cached_assert_datagram,
            (void *)assert_datagram,
            sizeof( AssertMessageDatagram_t ) );

    // Reset the 'assert string written' flag
    assert_datagram->magic_flag = MAGIC_FLAG_ASSERT_CLEARED;

    // Provide cached assert message to end-user pointer
    return (char *)&cached_assert_datagram.message;
}

/* ----- End ---------------------------------------------------------------- */
