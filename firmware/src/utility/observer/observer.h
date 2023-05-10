#ifndef OBSERVER_H
#define OBSERVER_H

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

#define OBSERVER_MAX_EVENT_COUNT 64

/* -------------------------------------------------------------------------- */

typedef uint8_t ObserverEvent_t;

typedef struct
{
    uint8_t type;       // describe the data being represented in the data union
    uint8_t index;      // helps distinguish different instances firing this event
    uint8_t spare;
    uint8_t spare2;
    uint32_t timestamp; // data timestamp TODO: should the timestamp be done at event level or data level?

    // Payload data 4-bytes, ergonomic access for common use-cases via union
    // TODO: consider a macro that grabs the right union based on the type value?
    union {
        float f32;
        uint32_t u32;
        int32_t i32;
    } data;
} EventData;

typedef void (*EventCallbackFn)(ObserverEvent_t event, EventData data, void *context);

typedef struct
{
    EventCallbackFn callback;
    void *context;
    uint64_t subscribed_events;  // supports up to 64 unique events, one per bit
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
