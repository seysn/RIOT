#include "js.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

void js_callback_isr(void *arg)
{
    js_callback_t *js_callback = (js_callback_t *)arg;
    DEBUG("trigger %p\n", (void *)js_callback);
    event_post(js_event_queue, &js_callback->event);
}

static void js_event_callback(event_t *event)
{
    js_callback_t *js_callback = (js_callback_t *) event;
    js_callback_run(js_callback);
}

void js_callback_init(js_callback_t *js_callback, jerry_value_t callback)
{
    DEBUG("init %p\n", (void *)js_callback);
    js_callback->event.handler = js_event_callback;

    DEBUG("acquire obj %lu\n", js_callback->callback.object);
    js_native_ref_add(&js_callback->callback, callback);
}

static void js_callback_cleanup(js_callback_t *js_callback)
{
    DEBUG("release %p\n", (void *)js_callback);
    js_native_ref_rem(&js_callback->callback);
}

void js_callback_run(js_callback_t *js_callback)
{
    DEBUG("run %p\n", (void *)js_callback);
    jerry_value_t callback = js_callback->callback.object;
    js_call_function(callback);
    js_callback_cleanup(js_callback);
}

void js_callback_cancel(js_callback_t *js_callback)
{
    DEBUG("cancel %p\n", (void *)js_callback);
    event_cancel(js_event_queue, &js_callback->event);
    js_callback_cleanup(js_callback);
}
