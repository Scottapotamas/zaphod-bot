#ifndef OBSERVER_H
#define OBSERVER_H

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

#define OBSERVER_MAX_EVENT_COUNT 32

/* -------------------------------------------------------------------------- */

//typedef enum
//{
//    SENSOR_EVENT_A = 0,
//    SENSOR_EVENT_B,
//
//    MAX_EVENT_COUNT = OBSERVER_MAX_EVENT_COUNT // MUST NOT EXCEED 32 EVENTS
//} EventFlag;

typedef uint8_t ObserverEvent_t;

typedef union
{
    float floatValue;
    uint32_t uint32Value;
    // Add more data types here if needed
} EventData;

typedef void (*EventCallbackFn)(ObserverEvent_t event, EventData data, void *context);

typedef struct
{
    EventCallbackFn callback;
    void *context;
    uint64_t subscribed_events;  // supports up to 32 unique events, one per bit
} Observer;

/* -------------------------------------------------------------------------- */

void observer_init( Observer *observer,
                    EventCallbackFn callback,
                    void *context
                 );

/* -------------------------------------------------------------------------- */

void observer_subscribe( Observer *observer,
                         ObserverEvent_t event
                         );

/* -------------------------------------------------------------------------- */

void observer_unsubscribe( Observer *observer,
                           ObserverEvent_t event
                           );

/* -------------------------------------------------------------------------- */

#endif    // OBSERVER_H
