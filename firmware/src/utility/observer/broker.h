#ifndef BROKER_H
#define BROKER_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "signals.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

#define BROKER_MAX_SUBSCRIBERS 20   /// Statically sized subscriber pool capacity

/* -------------------------------------------------------------------------- */

/**
 * @typedef ObserverEvent_t
 * @brief   Aliased definition for pub-sub topics from the system's 'signals' enum.
 */
typedef SYSTEM_EVENT_FLAG ObserverEvent_t;

/**
 * @enum    EVENT_DATA_INDEX_REF
 * @brief   Enum for improving ergonomics when referencing data in the event union arrays.
 */
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
    uint64_t subscribed_events; /// Bitfield of subscribed events, supports up to 64 unique events.
    QueueHandle_t queue;        /// Handle+storage for subscribed events to be queued.
} Subscriber;

/**
 * @struct  Broker
 * @brief   Acts as a singleton with a unrestricted input queue,
 * and a list of subscribers with queues for 'output.
 */
typedef struct
{
    SemaphoreHandle_t mutex;    /// Mutex for protecting read/write changes.
    QueueHandle_t queue;        /// Broker's input event queue
    Subscriber subscribers[BROKER_MAX_SUBSCRIBERS];  /// List of subscriber instances
} Broker;

/* -------------------------------------------------------------------------- */

/**
 * @brief   Initialize a broker.
 * This requires that the end-user instantiates a Broker structure themselves.
 * The input queue is automatically added to the FreeRTOS debug registry as "psBroker"
 * @param   *instance        Pointer to an existing Broker allocation which needs setup.
 * @param   rx_queue_depth   Input queue event capacity.
 */
PUBLIC void broker_init( Broker *instance, uint32_t rx_queue_depth );

/* -------------------------------------------------------------------------- */

/**
 * @brief   FreeRTOS task implementation for the broker.
 * The task is responsible for pulling events off the input queue and sending them to
 * downstream subscriber's queues as needed.
 * @param   *arg   Unused
 */
PUBLIC void broker_task( void *arg );

/* -------------------------------------------------------------------------- */

/**
 * @brief   Get the Broker instance in use.
 * As this implementation follows a singleton pattern,
 * this allows you to grab the currently active broker.
 * @returns Pointer to a Broker instance.
 */
PUBLIC Broker* broker_get_handle( void );

/* -------------------------------------------------------------------------- */

/**
 * @brief   Create/configure a subscriber stored in the Broker's sub pool.
 * Assumes a sub slot is available on the broker (or asserts).
 * @param   *name                   Name of the subscriber (used in the queue debug registry).
 * @param   subscriber_queue_depth  Depth of the subscriber queue.
 * @returns Pointer to the usable Subscriber instance.
 */
PUBLIC Subscriber * broker_create_subscriber( const char * name,
                                             size_t subscriber_queue_depth );

/* -------------------------------------------------------------------------- */

/**
 * @brief   Destroy a subscriber.
 * @param   *subscriber    Pointer to an existing Subscriber instance in the Broker's pool.
 * @returns True if the subscriber was successfully destroyed, false otherwise.
 */
PUBLIC bool broker_destroy_subscriber( Subscriber *subscriber );

/* -------------------------------------------------------------------------- */

/**
 * @brief   Publish an event to the system by adding it to the Broker's input queue.
 * @param   *evt   Pointer to a PublishedEvent instance.
 */
PUBLIC void broker_publish( PublishedEvent *evt );

/* -------------------------------------------------------------------------- */

/**
 * @brief   Subscribe to a topic
 * @param   *subscriber  Pointer to an existing Subscriber instance.
 * @param   event        Topic to subscribe to (by enum value)
 */
PUBLIC void broker_add_event_subscription( Subscriber *subscriber, ObserverEvent_t event );

/* -------------------------------------------------------------------------- */

/**
 * @brief   Remove a topic subscription from a subscriber.
 * @param   *subscriber  Pointer to an existing Subscriber instance.
 * @param   event        Topic to unsubscribe from (by enum value)
 */
PUBLIC void broker_remove_event_subscription( Subscriber *subscriber, ObserverEvent_t event );

/* -------------------------------------------------------------------------- */

#endif    // BROKER_H