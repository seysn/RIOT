APPLICATION = alphasense

BOARD = native

RIOTBASE ?= $(CURDIR)/../..

DEVELHELP ?=1

USEMODULE += js
USEMODULE += shell_commands
USEMODULE += posix
#USEMODULE += lwip_sock_tcp

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

USEMODULE += gnrc_ipv6_router_default

USEMODULE += gnrc_icmpv6_echo
USEMODULE += gcoap
USEMODULE += sock_util

USEMODULE += regression
USEMODULE += fmt

include $(CURDIR)/../../Makefile.include


JS_PATH := $(BINDIR)/js/$(MODULE)
# add directory of generated *.js.h files to include path
CFLAGS += -I$(JS_PATH)

# generate .js.h header files of .js files
JS = $(wildcard *.js)
JS_H := $(JS:%.js=$(JS_PATH)/%.js.h)

$(JS_PATH)/:
	@mkdir -p $@

$(JS_H): | $(JS_PATH)/
$(JS_H): $(JS_PATH)/%.js.h: %.js
	xxd -i $< | sed 's/^unsigned/const unsigned/g' > $@

$(RIOTBUILD_CONFIG_HEADER_C): $(JS_H)
