/*
 * Copyright (c) 2015-2016 Ken Bannister. All rights reserved.
 * Copyright (C) 2017 Inria
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/gcoap.h"
//#include "od.h"
//#include "fmt.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

extern char script[];
extern void js_restart(void);

ssize_t _blockwise_script_handler(coap_pkt_t* pkt, uint8_t *buf, size_t len)
{
    DEBUG("%s():l%u %s\n", __FILE__, __LINE__, __func__);

    uint32_t result = COAP_CODE_CHANGED;
    /* 204 : changd, answer to put or post request */
    uint32_t blknum;
    unsigned szx;
    /* szx : size of the block */
    int res = coap_get_blockopt(pkt, COAP_OPT_BLOCK1, &blknum, &szx);
    if (res >= 0) {
        printf("blknum=%u blksize=%u more=%u\n", (unsigned)blknum, coap_szx2size(szx), res);
        size_t offset = blknum << (szx + 4);
        printf("received bytes %u-%u\n", (unsigned)offset, (unsigned)offset+pkt->payload_len);

        /* overwrite the current script with the new received script  */
        memcpy(script + offset, (char *)pkt->payload, pkt->payload_len);
        if (res) {
            result = COAP_CODE_231;
        }
        else {
            script[offset + pkt->payload_len] = '\0';
            puts("script received (blockwise):");
            puts("-----");
            puts(script);
            puts("-----");
            puts("restarting js.");
            js_restart();
        }
    }
    else {
        memcpy(script, (char *)pkt->payload, pkt->payload_len);
        script[pkt->payload_len] = '\0';
        puts("script received:");
        puts("-----");
        puts(script);
        puts("-----");
        puts("restarting js.");
        js_restart();
    }

    ssize_t reply_len = coap_build_reply(pkt, result, buf, len, 0);
    uint8_t *pkt_pos = (uint8_t*)pkt->hdr + reply_len;
    if (res >= 0) {
        pkt_pos += coap_put_option_block1(pkt_pos, 0, blknum, szx, res);
    }
    return pkt_pos - (uint8_t*)pkt->hdr;
}


/* static ssize_t _riot_script_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len, */
/*                                     void *context) */
/* { */
/*     (void)context; */
/*     DEBUG("%s():l%u %s\n", __FILE__, __LINE__, __func__); */
/*     ssize_t rsp_len = 0; */
/*     unsigned code = COAP_CODE_EMPTY; */

/*     unsigned method_flag = coap_method2flag(coap_get_code_detail(pkt)); */

/*     switch (method_flag) { */
/*         case COAP_GET: */
/*             code = COAP_CODE_CONTENT; */
/*             rsp_len = strlen((char *)script); */
/*             break; */
/*         case COAP_POST: */
/*         case COAP_PUT: */
/*             return _blockwise_script_handler(pkt, buf, len); */
/*     } */

/*     return coap_reply_simple(pkt, code, buf, len, */
/*                              COAP_FORMAT_TEXT, (uint8_t *) script, rsp_len); */
/* } */

static const coap_resource_t _ressources[] = {
    /*
      Removed this part because it was doing a panic when calling it.
      I think the problem was because of the js library which was registering
      multiple times the ressource "/riot/js".
      But, this ressource is not used at the moment, so it's not a problem to
      remove it.
    */
    NULL
    /* { "/riot/script", COAP_GET | COAP_PUT | COAP_POST, _riot_script_handler, NULL}, */
};
static gcoap_listener_t _listener = {
    (coap_resource_t *)&_ressources[0],
    sizeof(_ressources) / sizeof(_ressources[0]),
    NULL
};

void server_init(void)
{
    gcoap_register_listener(&_listener);
}
