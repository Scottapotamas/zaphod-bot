/* -------------------------------------------------------------------------- */

#include "broker.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

/**
 * @brief   Finds the first unused subscriber slot in the Broker's pool storage.
 *          Uses a dumb linear search.
 *          A subscriber slot is considered empty if the queue handle isn't setup.
 * @returns Slot array index value, or SIZE_MAX if failed.
 */
PRIVATE size_t broker_find_empty_subscriber_slot( void );

/**
 * @brief   Finds the matching subscription entry in the broker's pool
 *          Uses a dumb linear search.
 * @param   Pointer to an existing Subscriber stored in the pool
 * @returns Slot array index value, or SIZE_MAX if failed.
 */
PRIVATE size_t broker_find_subscriber_slot( Subscriber *slot_ptr );

/* -------------------------------------------------------------------------- */

/// Local pointer to the end-user's broker instance
PRIVATE Broker *pubsub_broker = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void broker_init( Broker *instance, uint32_t rx_queue_depth )
{
    pubsub_broker = instance;

    // Setup the input queue
    pubsub_broker->queue = xQueueCreate( rx_queue_depth, sizeof(PublishedEvent) );
    ENSURE( pubsub_broker->queue );
    vQueueAddToRegistry( pubsub_broker->queue, "psBroker");  // Debug view annotation

    // Setup broker data protection mutex
    pubsub_broker->mutex = xSemaphoreCreateMutex();
    ENSURE( pubsub_broker->mutex );
}

/* -------------------------------------------------------------------------- */

PUBLIC void broker_task( void *arg )
{
    REQUIRE( pubsub_broker );
    REQUIRE( pubsub_broker->mutex );
    REQUIRE( pubsub_broker->queue );

    PublishedEvent inbound_event = { 0 };

    for(;;)
    {
        // Block until events hit the input queue
        xQueueReceive( pubsub_broker->queue, &inbound_event, portMAX_DELAY );

        // Grab mutex to ensure subscribers aren't modified while handling
        if( xSemaphoreTake( pubsub_broker->mutex, portMAX_DELAY ) )
        {
            // Walk the list of potential subscribers
            for( size_t sub_index = 0; sub_index < BROKER_MAX_SUBSCRIBERS; sub_index++ )
            {
                Subscriber *sub = &pubsub_broker->subscribers[sub_index];

                // Active subscription to this event topic?
                if( sub->queue && (sub->subscribed_events & ((uint64_t)1 << inbound_event.topic)) )
                {
                    // Dispatch the event to the subscriber's queue
                    ENSURE( xQueueSendToBack( sub->queue, &inbound_event, 0 ) );
                }
            }

            xSemaphoreGive( pubsub_broker->mutex );
        }
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC Broker* broker_get_handle( void )
{
    return pubsub_broker;
}

/* -------------------------------------------------------------------------- */

PUBLIC Subscriber * broker_create_subscriber( const char * name, size_t subscriber_queue_depth )
{
    REQUIRE( pubsub_broker->mutex );
    Subscriber *sub = NULL;

    if( xSemaphoreTake( pubsub_broker->mutex, portMAX_DELAY ) )
    {
        // Grab a free slot
        uint32_t slot_index = broker_find_empty_subscriber_slot();

        // Create our subscriber
        sub = &pubsub_broker->subscribers[slot_index];
        sub->subscribed_events = 0;

        // Setup the output queue
        sub->queue = xQueueCreate( subscriber_queue_depth, sizeof( PublishedEvent ) );
        REQUIRE( sub->queue );
        vQueueAddToRegistry( sub->queue, name );    // Debug view annotation

        xSemaphoreGive( pubsub_broker->mutex );
    }

    return sub;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool broker_destroy_subscriber( Subscriber *subscriber )
{
    REQUIRE( pubsub_broker->mutex );
    bool destroyed = false;

    if( xSemaphoreTake( pubsub_broker->mutex, portMAX_DELAY ) )
    {
        // Find the subscriber pool entry by pointer
        if( broker_find_subscriber_slot( subscriber ) < BROKER_MAX_SUBSCRIBERS )
        {
            // It exists in the pool, so wipe it out
            subscriber->subscribed_events = 0;

            // Deletion also covers debug annotation removal
            vQueueDelete( subscriber->queue );

            destroyed = true;
        }

        xSemaphoreGive( pubsub_broker->mutex );
    }

    return destroyed;
}

/* -------------------------------------------------------------------------- */

PUBLIC void broker_publish( PublishedEvent *evt )
{
    // Add the event to the broker's input queue
    ENSURE( xQueueSendToBack( pubsub_broker->queue, evt, 0 ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void broker_add_event_subscription( Subscriber *subscriber, ObserverEvent_t event )
{
    REQUIRE( subscriber );
    REQUIRE( pubsub_broker );
    REQUIRE( event < SYSTEM_NUM_FIELDS);

    if( xSemaphoreTake( pubsub_broker->mutex, portMAX_DELAY ) )
    {
        // Check the pointer to the subscriber matches one of the broker's entries
        REQUIRE( broker_find_subscriber_slot( subscriber ) < BROKER_MAX_SUBSCRIBERS );

        // Set the event bit
        subscriber->subscribed_events |= ( (uint64_t)1 << event );

        xSemaphoreGive( pubsub_broker->mutex );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void broker_remove_event_subscription( Subscriber *subscriber, ObserverEvent_t event )
{
    REQUIRE( subscriber );
    REQUIRE( pubsub_broker );

    if( xSemaphoreTake( pubsub_broker->mutex, portMAX_DELAY ) )
    {
        // Check the pointer to the subscriber matches one of the broker's entries
        REQUIRE( broker_find_subscriber_slot( subscriber ) < BROKER_MAX_SUBSCRIBERS );

        // Unset the event bit
        subscriber->subscribed_events &= ~( (uint64_t)1 << event );

        xSemaphoreGive( pubsub_broker->mutex );
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE size_t broker_find_empty_subscriber_slot( void )
{
    REQUIRE( pubsub_broker );

    for( size_t i = 0; i < BROKER_MAX_SUBSCRIBERS; i++ )
    {
        if( pubsub_broker->subscribers[i].queue == NULL )
        {
            return i;
        }
    }

    ASSERT_PRINTF( false, "Broker exhausted sub slots" );
    return SIZE_MAX;
}

/* -------------------------------------------------------------------------- */

PRIVATE size_t broker_find_subscriber_slot( Subscriber *slot_ptr )
{
    REQUIRE( pubsub_broker );

    for( size_t i = 0; i < BROKER_MAX_SUBSCRIBERS; i++ )
    {
        if( &pubsub_broker->subscribers[i] == slot_ptr )
        {
            return i;
        }
    }

    return SIZE_MAX;
}

/* -------------------------------------------------------------------------- */
