#include "observer.h"
#include "qassert.h"

DEFINE_THIS_FILE;

void observer_init( Observer *observer,
                    EventCallbackFn callback,
                    void *context)
{
    REQUIRE( observer );
    REQUIRE( callback );

    observer->callback = callback;
    observer->context = context;
    observer->subscribed_events = 0;
}

void observer_subscribe( Observer *observer, EventFlag event)
{
    REQUIRE( observer );
    REQUIRE( event < MAX_EVENT_COUNT );

    observer->subscribed_events |= (1 << event); // Set the event bit
}

void observer_unsubscribe( Observer *observer, EventFlag event)
{
    REQUIRE( observer );
    REQUIRE( event < MAX_EVENT_COUNT );

    observer->subscribed_events &= ~(1 << event); // Unset the event bit
}


