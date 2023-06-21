#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/**
 * @brief Initializes the buzzer module.
 *
 * Must be run before using other buzzer functionality
 */
PUBLIC void buzzer_init( void );

/* -------------------------------------------------------------------------- */

/**
 * @brief FreeRTOS task for the buzzer.
 *
 * Handles the buzzer queue and state-machine responsible for starting and stopping tones,
 * and waiting between subsequent tones.
 *
 * @param arg Unused.
 */
PUBLIC void buzzer_task( void *arg );

/* -------------------------------------------------------------------------- */

/**
 * @brief Enqueue a sound with the buzzer.
 *
 * This function adds a new sound event to the buzzer's task queue.
 * When requesting multiple beeps, the beep duration is also used for the 'silent' time.
 *
 * @param count Number of beeps to generate.
 * @param frequency Frequency of the buzzer sound in Hz.
 * @param duration_ms Duration of each beep in milliseconds.
 */
PUBLIC void buzzer_sound( uint8_t count, uint16_t frequency, uint16_t duration_ms );

/* -------------------------------------------------------------------------- */

/**
 * @brief Sets the mute state of the buzzer.
 *
 * @param muted `true` to mute the buzzer, `false` to unmute.
 */
PUBLIC void buzzer_mute( bool muted );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* BUZZER_H */
