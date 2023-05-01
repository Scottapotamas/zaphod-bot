#ifndef OBSERVER_H
#define OBSERVER_H

#include "global.h"

typedef enum
{
    SENSOR_EVENT_A = 0,
    SENSOR_EVENT_B,

    MAX_EVENT_COUNT = 32 // MUST NOT EXCEED 32 EVENTS
} EventFlag;

typedef union
{
    float floatValue;
    uint32_t uint32Value;
    // Add more data types here if needed
} EventData;

typedef void (*EventCallbackFn)(EventFlag event, EventData data, void *context);

typedef struct
{
    EventCallbackFn callback;
    void *context;
    uint32_t subscribed_events;  // supports up to 32 unique events, one per bit
} Observer;

void observer_init( Observer *observer,
                    EventCallbackFn callback,
                    void *context
                 );

void observer_subscribe( Observer *observer,
                         EventFlag event
                         );

void observer_unsubscribe( Observer *observer,
                           EventFlag event
                           );

#endif    // OBSERVER_H
