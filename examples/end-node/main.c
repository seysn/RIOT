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

#ifndef TEST_LPS331AP_I2C
#error "TEST_LPS331AP_I2C not defined"
#endif
#ifndef TEST_LPS331AP_ADDR
#error "TEST_LPS331AP_ADDR not defined"
#endif

#include <stdio.h>

#include "net/gcoap.h"
#include "random.h"
#include "xtimer.h"
#include "timex.h"
#include "shell.h"
#include "fmt.h"
#include "thread.h"
#include "lps331ap.h"

#define RATE        LPS331AP_RATE_7HZ

#define ENABLE_DEBUG (1)
#include "debug.h"

#define INTERVAL (1U * US_PER_SEC)
#define MAIN_QUEUE_SIZE (4)

int envoie_valeur(int argc, char **argv);
static void _resp_handler(unsigned req_state, coap_pkt_t *pdu,
                          sock_udp_ep_t *remote);
static size_t _send(uint8_t *buf, size_t len, ipv6_addr_t addr, uint16_t port);
void *thread_send(void *arg);

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static const shell_command_t shell_commands[] = {
    { "send", "send value", envoie_valeur },
    { NULL, NULL, NULL }
};
static char remote_addr_str[] = "2001:660:4403:484:1711:6b10:65f9:ac2a";
char thread_send_stack[THREAD_STACKSIZE_MAIN];
static lps331ap_t dev;

int main(void)
{
    printf("Initializing LPS331AP sensor at I2C_%i...", TEST_LPS331AP_I2C);
    if (lps331ap_init(&dev, TEST_LPS331AP_I2C, TEST_LPS331AP_ADDR, RATE) == 0) {
        puts("[Initialization OK]");
    }
    else {
        puts("[Initialization FAILED]");
    }
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    random_init(xtimer_now().ticks32);
    puts("CoAP end node example");
    puts("Starting the shell now");
    thread_create(thread_send_stack, sizeof(thread_send_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  thread_send, NULL, "thread_send");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

int envoie_valeur(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    ipv6_addr_t remote_addr;
    int temp, pres;
    int temp_abs, pres_abs;
    puts("Request initialisation ...");
    gcoap_req_init(&pdu, &buf[0], GCOAP_PDU_BUF_SIZE,
                   COAP_METHOD_PUT, "/riot/value");
    puts("Set request as CON type");
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    puts("Finishing building the request");
    pres = lps331ap_read_pres(&dev);
    /* Retourne la pression en mbar */
    temp = lps331ap_read_temp(&dev);
    /* Retourne la température en m °C */
    pres_abs = pres / 1000;
    pres -= pres_abs * 1000;
    temp_abs = temp / 1000;
    temp -= temp_abs * 1000;
    printf("Pressure value: %2i.%03i bar - Temperature: %2i.%03i °C\n",
           pres_abs, pres, temp_abs, temp);
    fmt_u32_dec((char *)pdu.payload, pres);
    len = strlen((char*)pdu.payload);
    len = gcoap_finish(&pdu, len, COAP_FORMAT_TEXT);
    if (ipv6_addr_from_str(&remote_addr, remote_addr_str) == NULL) {
        puts("Unable to parse destination address");
        return -1;
    }
    size_t bytes_sent = _send(buf, len, remote_addr, GCOAP_PORT);
    if (bytes_sent > 0) {
        puts("Sucessfuly send the value");
    }
    return bytes_sent;
}

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu,
                          sock_udp_ep_t *remote)
{
    (void)req_state;
    (void)pdu;
    (void)remote;
    printf("Response handler !\n");
}

static size_t _send(uint8_t *buf, size_t len, ipv6_addr_t addr, uint16_t port)
{
        size_t bytes_sent;
        sock_udp_ep_t remote;

        remote.family = AF_INET6;
        remote.netif = SOCK_ADDR_ANY_NETIF;

        if ((remote.netif == SOCK_ADDR_ANY_NETIF) && ipv6_addr_is_link_local(&addr)) {
            puts("send: must specify interface for link local target");
            return 0;
        }
        memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

        remote.port = port;

        bytes_sent = gcoap_req_send2(buf, len, &remote, _resp_handler);

        return bytes_sent;
}


void *thread_send(void *arg)
{
    /* thread that wakes up every seconds */
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while(1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        puts("Wake up !");
        envoie_valeur(0, NULL);
    }
}
