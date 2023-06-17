#ifndef BROKER_H
#define BROKER_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "signals.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

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
    } value;
} StampedData_t;

typedef struct
{
    uint32_t topic;
    union {
        StampedData_t stamped;
        uint32_t      u_triple[EVT_DIM];
        int32_t       s_triple[EVT_DIM];
        float         f_triple[EVT_DIM];
    } data;
} PublishedEvent;

// Concept of a subscriber is a queue handle, and the list/bitfield of events it's subscribed to
typedef struct
{
    uint64_t subscribed_events; // supports up to 64 unique events
    QueueHandle_t queue;
} Subscriber;

#define BROKER_MAX_SUBSCRIBERS 20

typedef struct
{
    SemaphoreHandle_t mutex;    // protect read/write changes
    QueueHandle_t queue;        // input queue
    Subscriber subscribers[BROKER_MAX_SUBSCRIBERS];  // list of subscriber instances
} Broker;

/* -------------------------------------------------------------------------- */

PUBLIC void broker_init( Broker *instance );

/* -------------------------------------------------------------------------- */

PUBLIC void broker_task( void *arg );

/* -------------------------------------------------------------------------- */

PUBLIC Broker* broker_get_handle( void );

/* -------------------------------------------------------------------------- */

PUBLIC Subscriber * broker_create_subscriber( const char * name,
                                             size_t subscriber_queue_depth );

/* -------------------------------------------------------------------------- */

PUBLIC bool broker_destroy_subscriber( Subscriber *subscriber );

/* -------------------------------------------------------------------------- */

PUBLIC void broker_publish( PublishedEvent *evt );

/* -------------------------------------------------------------------------- */

PUBLIC void broker_add_event_subscription( Subscriber *subscriber, ObserverEvent_t event );

/* -------------------------------------------------------------------------- */

PUBLIC void broker_remove_event_subscription( Subscriber *subscriber, ObserverEvent_t event );

/* -------------------------------------------------------------------------- */

#endif    // BROKER_H