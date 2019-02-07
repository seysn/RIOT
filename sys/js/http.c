#include <string.h>
#include <stdio.h>

#include "event.h"
#include "js.h"
#include "net/af.h"
#include "net/ipv4/addr.h"
#include "net/ipv6/addr.h"
#include "net/sock/tcp.h"

static JS_EXTERNAL_HANDLER(http_get_ipv4) {
    (void)func_value;
    (void)this_value;

    uint8_t buf[128];
    sock_tcp_t sock;
    int res;
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;

    if (args_cnt < 2) {
        puts("http.get_ipv4(): not enough arguments");
        return 0;
    }

    if (!jerry_value_is_string(args_p[0])) {
        puts("saul.get_ipv4(): arg 0 not a string");
        return 0;
    }

    if (!jerry_value_is_string(args_p[1])) {
        puts("saul.get_ipv4(): arg 1 not a string");
        return 0;
    }

    char host[128];
    memset(host, '\0', sizeof(host));
    jerry_string_to_char_buffer(args_p[0], (jerry_char_t*)host, sizeof(host));

    char content[4096];
    memset(content, '\0', sizeof(content));
    jerry_string_to_char_buffer(args_p[0], (jerry_char_t*)content, sizeof(content));

    remote.port = 80;
    ipv4_addr_from_str((ipv6_addr_t *)&remote.addr, host);
    if (sock_tcp_connect(sock, &remote, 0, 0) < 0) {
        puts("Error connecting sock");
        return 1;
    }
    if ((res = sock_tcp_write(sock, content, strlen(content))) < 0) {
        puts("Errored on write");
    } else {
        if ((res = sock_tcp_read(sock, &buf, sizeof(buf),
                                 SOCK_NO_TIMEOUT)) < 0) {
            puts("Disconnected");
        }

        // Testing, needs to be returned
        printf("Read: \"");
        for (int i = 0; i < res; i++) {
            printf("%c", buf[i]);
        }
        puts("\"");
    }
    sock_tcp_disconnect(sock);
    return res;
}

const js_native_method_t http_methods[] =
{
 { "_get_ipv4", js_external_handler_http_get_ipv4 }
};

const unsigned http_methods_len = sizeof(http_methods) / sizeof(http_methods[0]);
