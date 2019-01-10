/*
 * Copyright (C) 2017 Inria
 *               2017 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Anthony app
 *
 * @author      Anthony Durot <anthony.durot@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "msg.h"
#include "xtimer.h"
#include "js.h"
#include "regression.h"
#include "fmt.h"
#include "fix16.h"

#include "server.h"
#include "lib.js.h"
#include "main.js.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

static event_queue_t event_queue;

char script[2048];

#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

extern int _gnrc_netif_config(int argc, char **argv);

static int lib_init = 0;

void js_start(event_t *unused) {
    (void)unused;
    size_t script_len = strlen(script);
    puts("(re)initializng jerryscript engine");
    js_init();

    if (lib_init == 0) {
        puts("Executing lib script");
        js_run(lib_js, lib_js_len);
        //lib_init = 1;
    }

    /* puts("Executing main script..."); */
    /* js_run(main_js, main_js_len); */
    if (script_len) {
        puts("Executing script...");
        js_run((jerry_char_t *)script, script_len);
    }
    else {
        puts("Empty script, nothing to execute yet.");
    }
}


static event_t js_start_event = { .handler = js_start };

void js_restart(void) {
    DEBUG("%s:l%u:%s()\n", __FILE__, __LINE__, __func__);
    js_shutdown(&js_start_event);
}


int main(void) {
    /* polynom_t polynom = { .a = 1, .b = 0 , .start_time = xtimer_now() }; */
    polynom_t *polynom = NULL;
    printf("You are running RIOT on a(n) %s board. \n", RIOT_BOARD);
    printf("This board features a(n) %s MCU. \n", RIOT_MCU);
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    data_t data1 = { .value = 0, .timestamp = xtimer_now() };

    puts("waiting for network config");
    xtimer_sleep(3);

    data_t data2 = { .value = 3.0, .timestamp = xtimer_now() };


    puts("Configured network interfaces:");
    _gnrc_netif_config(0, NULL);

    puts("starting gcoap server");
    server_init();


    puts("Test add data");

    printf("Intial stata : polynom points to %p \n", (void *)polynom);

    add_value(&polynom, &data1);

    printf("Afer first value : polynom points to %p \n", (void *)polynom);

    add_value(&polynom, &data2);

    free(polynom);


    puts("setting up event queue");
    event_queue_init(&event_queue);
    js_event_queue = &event_queue;
    puts("Executing js...");
    js_restart();
    puts("Entering event loop...");
    event_loop(&event_queue);

    return 0;
}
