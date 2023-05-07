#ifndef SUBJECT_H
#define SUBJECT_H

/* -------------------------------------------------------------------------- */

#include "observer.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* -------------------------------------------------------------------------- */

#define MAX_OBSERVERS 10

/* -------------------------------------------------------------------------- */

typedef struct {
    Observer *observers[MAX_OBSERVERS];
    SemaphoreHandle_t mutex;
} Subject;

/* -------------------------------------------------------------------------- */

void subject_init(Subject *subject);

/* -------------------------------------------------------------------------- */

bool subject_add_observer(Subject *subject, Observer *observer);

/* -------------------------------------------------------------------------- */

bool subject_remove_observer(Subject *subject, Observer *observer);

/* -------------------------------------------------------------------------- */

void subject_notify(Subject *subject, ObserverEvent_t event, EventData data);

/* -------------------------------------------------------------------------- */

#endif    // SUBJECT_H
