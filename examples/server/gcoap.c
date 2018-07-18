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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "net/gcoap.h"
#include "fmt.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

#define MAX_VALUES 50

/* _random_handler de type coap_handler_t */
static ssize_t _value_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                               void *ctx);
static void array_to_string(char *out, uint32_t array[]);

static const coap_resource_t _resources[] = {
    { "/riot/value", COAP_PUT | COAP_POST | COAP_GET, _value_handler, NULL}
};

static gcoap_listener_t _listener = {
    (coap_resource_t *)&_resources[0],
    sizeof(_resources) / sizeof(_resources[0]),
    NULL
};

uint8_t request_number = 0;
uint32_t values_array[MAX_VALUES];

static ssize_t _value_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                            void *ctx)
{
    DEBUG("Random handler triggered\n");
    (void)ctx;
    uint32_t value;
    unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));
    char *payload;
    switch(method_flag) {
        case COAP_GET:
            /* TODO */
            gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
            strcpy((char *)pdu->payload, "");
            array_to_string((char *)pdu->payload, values_array);
            size_t payload_len = strlen((char *)pdu->payload);
            printf("Le payload est %s\n",(char *)pdu->payload);
            return gcoap_finish(pdu, payload_len, COAP_FORMAT_TEXT);
            break;
        case COAP_PUT:
        case COAP_POST:
            /* TODO */
            if(pdu->payload_len >= 1 && request_number < MAX_VALUES) {
                payload = malloc(pdu->payload_len * sizeof(char));
                memcpy(payload, (char *)pdu->payload, pdu->payload_len);
                value = (uint32_t)strtoull(payload, NULL, 10);
                free(payload);
                values_array[request_number] = value;
                request_number++;
                return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
            }
            else {
                printf("Il y a trop de valeurs \n");
                return gcoap_response(pdu, buf, len, COAP_CODE_UNAUTHORIZED);
            }
            break;
        default:
            return 0;
            break;
    }

    return 0;
}

void gcoap_server_init(void)
{
    DEBUG("Register listener\n");
    gcoap_register_listener(&_listener);
}

static void array_to_string(char *out, uint32_t array[])
{
    int i = 0;
    char actual_number[5];
    for(i = 0 ; i < request_number ; i++) {
        fmt_u32_dec((char *)actual_number, array[i]);
        strcat(out, (char *)actual_number);
        if(i != request_number - 1){
                strcat(out, " - ");
        }
    }
    strcat(out, '\0');
}
