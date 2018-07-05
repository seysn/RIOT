APPLICATION = end-node
BOARD ?= native
RIOTBASE ?= $(CURDIR)/../..

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

USEMODULE += gnrc_ipv6_default
USEMODULE += gcoap

USEMODULE += xtimer
USEMODULE += fmt
USEMODULE += random

USEMODULE += shell
USEMODULE += shell_commands

DEVELHELP ?= 1

QUIET ?= 1

include $(RIOTBASE)/Makefile.include
