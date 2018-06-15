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
 * @brief       Example of how to use javascript on RIOT
 *
 * @author      Emmanuel Baccelli <emmanuel.baccelli@inria.fr>
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>

#include "js.h"

/* include headers generated from *.js */
#include "main.js.h"


void js_start(void)
{

    if (main_js_len) {
        puts("(re)initializing jerryscript engine...");
        js_init();

        puts("Executing script...");
        js_run(main_js, main_js_len);
    }
    else {
        puts("Emtpy script, nothing to execute yet.");
    }
}


int main(void)
{
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);
    js_start();

    return 0;
}
