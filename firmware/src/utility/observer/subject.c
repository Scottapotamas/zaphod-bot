#include "subject.h"
#include "qassert.h"

DEFINE_THIS_FILE;

void subject_init( Subject *subject )
{
    REQUIRE( subject );

    for( int i = 0; i < MAX_OBSERVERS; i++ )
    {
        subject->observers[i] = NULL;
    }

    subject->mutex = xSemaphoreCreateMutex();
    ENSURE( subject->mutex );
}

bool subject_add_observer( Subject *subject, Observer *observer )
{
    REQUIRE( subject );
    REQUIRE( observer );
    bool result = false;

    xSemaphoreTake(subject->mutex, portMAX_DELAY);

    // TODO: Consider checking for duplicate observers if paranoid about correctness?
    for( int i = 0; i < MAX_OBSERVERS; i++ )
    {
        if( subject->observers[i] == NULL )
        {
            subject->observers[i] = observer;
            result = true;
            break;
        }
    }

    xSemaphoreGive(subject->mutex);
    return result;
}

bool subject_remove_observer( Subject *subject, Observer *observer )
{
    REQUIRE( subject );
    REQUIRE( observer );
    bool result = false;

    xSemaphoreTake(subject->mutex, portMAX_DELAY);

    for( int i = 0; i < MAX_OBSERVERS; i++)
    {
        if( subject->observers[i] == observer)
        {
            subject->observers[i] = NULL;
            result = true;
            break;
        }
    }

    xSemaphoreGive(subject->mutex);
    return result;
}

void subject_notify(Subject *subject, ObserverEvent_t event, EventData data)
{
    REQUIRE( subject );
    REQUIRE( event < OBSERVER_MAX_EVENT_COUNT );

    xSemaphoreTake( subject->mutex, portMAX_DELAY );

    for( int i = 0; i < MAX_OBSERVERS; i++ )
    {
        Observer *observer = subject->observers[i];

        if( observer && (observer->subscribed_events & (1 << event)) )
        {
            observer->callback(event, data, observer->context);
        }
    }

    xSemaphoreGive(subject->mutex);
}

