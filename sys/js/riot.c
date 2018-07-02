#include "js.h"
#include "periph/pm.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

static JS_EXTERNAL_HANDLER(reboot) {
    (void)func_value;
    (void)this_value;
    (void)args_p;
    (void)args_cnt;

    pm_reboot();

    return 0;
}

static JS_EXTERNAL_HANDLER(echo) {
    (void)func_value;
    (void)this_value;
    if (args_cnt < 1) {
        puts("riot.echo(): not enough arguments");
        return 0;
    }

    if (!jerry_value_is_string(args_p[0])) {
        puts("riot.echo(): arg 0 not a string");
        return 0;
    }

    char name[256];

    if(!jerry_string_to_char_buffer(args_p[0], (jerry_char_t *)name, sizeof(name))){
        puts("riot.echo(): unable to allocate string");
    }

    printf("Your argument was : %s\n", name);
    return 0;
}

const js_native_method_t riot_methods[] = {
    { "reboot", js_external_handler_reboot },
    { "echo", js_external_handler_echo}
};

const unsigned riot_methods_len = sizeof(riot_methods)/sizeof(riot_methods[0]);
