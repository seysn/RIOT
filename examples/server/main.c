/*
 * Copyright (C) 2014 Freie Universität Berlin
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
 * @brief       Anthony application
 *
 * @author      Anthony Durot <anthony.durot@inria.fr>
 *
 * @}
 */

#include <stdio.h>

#include "random.h"
#include "xtimer.h"

extern int _gnrc_netif_config(int argc, char **argv);
extern void gcoap_server_init(void);

int main(void)
{
    puts("CoAP random server example");
    puts("Waiting for network config...");
    xtimer_sleep(3);
    puts("Configured network interfaces :");
    _gnrc_netif_config(0, NULL);
    gcoap_server_init();
    while(1){}
    return 0;
}
