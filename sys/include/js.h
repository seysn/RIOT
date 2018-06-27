/**
 * @degroup    middleware
 * @ingroup    net
 * @brief      Middleware Javascript
 *
 * middleware provide a high-level interface for writing Javascript applications
 * wich interracts with RIOT. It is also a wrapper arround JerryScript engine
 * functions
 *
 * @{
 *
 * @file
 * @brief    middleware definition
 *
 * @author    Anthony Durot <anthony.durot@inria.fr>
 */

#ifndef JS_H
#define JS_H

#include "clist.h"
#include "event.h"
#include "jerryscript.h"
#include "jerryscript-ext/handler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Queue for javascript events
 */

extern event_queue_t *js_event_queue;

/**
 * @brief Represents a method for a Javascript object
 */
typedef struct {
    const char *name;                           /**< name of the method */
    jerry_external_handler_t handler;           /**< handler of the method */
} js_native_method_t;

/**
 * @brief Represents natives objects
 */
typedef struct {
    const char *name;                           /**< name of the native object */
    const js_native_method_t *methods;          /**< list of methods */
} js_native_objects_t;

/**
 * @brief
 */
typedef struct {
    list_node_t ref;
    jerry_value_t object;
} js_native_ref_t;

/**
 * @brief
 */
typedef struct {
    event_t event;
    js_native_ref_t callback;
} js_callback_t;

/**
 * @brief Starts the Javascript engine
 */
void js_init(void);

/**
 * @brief Run a script
 *
 * @param [in] script           Script to run
 * @param [in] script_size      Length of the script
 *
 * @return 0 on success
 * @return < 0 on error
 */
int js_run(const jerry_char_t *script, size_t script_size);

/**
 * @brief add a native reference
 *
 * @param [in] ref              Native reference to add
 * @param [in] object           Object to add
 */
void js_native_ref_add(js_native_ref_t *ref, jerry_value_t object);

/**
 * @brief remove a native reference
 *
 * @param [in] ref              Native reference to remove
 */
void js_native_ref_rem(js_native_ref_t *ref);

/**
 * @brief initalize the objects
 */
void js_init_objects(void);

/**
 * @brief add an external handler to an object with a name
 *
 * @param [in] object       Object on witch you add the handler
 * @param [in] name         Name of the handler
 * @param [in] handler      Handler function
 */
void js_add_external_handler(jerry_value_t object, const char *name, jerry_external_handler_t handler);

/**
 * @brief add an object to another with a name
 *
 * @param [in] object       Object on witch you add the other object
 * @param [in] other        Object that is added
 * @param [in] name         Name of the object
 */
void js_add_object(jerry_value_t object, jerry_value_t other, const char *name);

/**
 * @brief
 *
 * @param
 * @param
 * @param
 */
void *js_get_object_native_pointer(jerry_value_t object, const jerry_object_native_info_t *type);

/**
 * @brief create an object with specific methods
 *
 * @param [in] methods      List of methods the object will have
 * @param [in] num_methods  Number of methods the object will have
 */
jerry_value_t js_object_create_with_methods(const js_native_method_t *methods, unsigned num_methods);
jerry_value_t js_object_native_create(size_t size, const jerry_object_native_info_t *native_obj_type_info);
void js_callback_isr(void *arg);
void js_call_function(jerry_value_t callback);
void js_callback_init(js_callback_t *js_callback, jerry_value_t callback);
void js_callback_run(js_callback_t *js_callback);
void js_callback_cancel(js_callback_t *callback);
void js_shutdown(event_t *shutdown_done_event);
char *js_strdup(jerry_value_t string);

double js_object_get_number(jerry_value_t object, const char *name);
jerry_value_t js_get_property(jerry_value_t object, const char *name);

#define js_check(x) if (jerry_value_is_error(x)) printf("%s:%u: js_check("#x") failed!\n", __FILE__, __LINE__)

#define JS_EXTERNAL_HANDLER(name) \
    jerry_value_t \
    js_external_handler_ ## name (const jerry_value_t func_value, \
                                const jerry_value_t this_value, \
                                const jerry_value_t * args_p, \
                                const jerry_length_t args_cnt)

#ifdef __cplusplus
}
#endif

#endif /* JS_H */
/** @} */
