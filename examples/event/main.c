#include <stdio.h>

#include "event.h"

typedef struct {
    event_t super;
    const char *text;
} custom_event_t;

static void handler(event_t *event)
{
    printf("Triggered event 0x%08x\n", (unsigned)event);
}

static void custom_handler(event_t *event)
{
    custom_event_t *custom_event = (custom_event_t *)event;
    printf("Triggered custom event with text : \"%s\"\n", custom_event->text);
}


/*
An event is a structure containing a pointer to an event holder
it can be extended to procide context or arguments to the handler.
It can also be embedded into existing structures
*/
static event_t event = { .handler = handler};
static custom_event_t custom_event = { .super.handler = custom_handler, .text = "CUSTOM EVENT"};
static custom_event_t custom_event_forbidden = { .super.handler = custom_handler, .text = "SHOULD NOT BE REACHED"};

/*
An event queue is basically a FIFO queue of events, with some functions to
efficiently and safely handle adding and getting events to / from such a queue.
An event queue is boud to a thread, but any thread or ISR can put events into a
queue
*/

static event_queue_t queue;

int main(void)
{
    event_queue_init(&queue);
    event_post(&queue, &event);
    event_post(&queue, (event_t *)&custom_event);
    event_loop(&queue);
    event_post(&queue, (event_t *)&custom_event_forbidden);
    return 0;
}
