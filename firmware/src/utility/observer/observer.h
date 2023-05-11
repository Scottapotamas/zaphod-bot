#ifndef OBSERVER_H
#define OBSERVER_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "signals.h"

/* -------------------------------------------------------------------------- */

#define OBSERVER_MAX_EVENT_COUNT 64

/* -------------------------------------------------------------------------- */

typedef SYSTEM_EVENT_FLAG ObserverEvent_t;

// array
// TODO: can this behaviour be formalised in a more correct/expressive manner?
typedef enum
{
    // Triplets for position/angles, no timestamp.
    EVT_X = 0,
    EVT_Y = 1,
    EVT_Z = 2,

    EVT_A = 0,
    EVT_B = 1,
    EVT_C = 2,

    EVT_DIM = 3,
} EVENT_DATA_INDEX_REF;

typedef struct
{
    uint32_t timestamp;
    uint32_t index;
    union
    {
        uint32_t u32;
        int32_t  i32;
        float    f32;
    } data;
} StampedData_t;

typedef union
{
    StampedData_t stamped;
    uint32_t      u_triple[EVT_DIM];
    int32_t       s_triple[EVT_DIM];
    float         f_triple[EVT_DIM];
} EventData;

typedef void ( *EventCallbackFn )( ObserverEvent_t event, EventData data, void *context );

typedef struct
{
    EventCallbackFn callback;
    void           *context;
    uint64_t        subscribed_events;    // supports up to 64 unique events, one per bit
} Observer;

/* -------------------------------------------------------------------------- */

void observer_init( Observer       *observer,
                    EventCallbackFn callback,
                    void           *context );

/* -------------------------------------------------------------------------- */

void observer_subscribe( Observer       *observer,
                         ObserverEvent_t event );

/* -------------------------------------------------------------------------- */

void observer_unsubscribe( Observer       *observer,
                           ObserverEvent_t event );

/* -------------------------------------------------------------------------- */

#endif    // OBSERVER_H
