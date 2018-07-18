APPLICATION = end-node
BOARD ?= native
RIOTBASE ?= $(CURDIR)/../..

FEATURES_REQUIRED = periph_i2c

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

USEMODULE += gnrc_ipv6_default
USEMODULE += gcoap

USEMODULE += xtimer
USEMODULE += fmt
USEMODULE += random

USEMODULE += shell
USEMODULE += shell_commands

USEMODULE += lps331ap

DEVELHELP ?= 1

QUIET ?= 1

TEST_LPS331AP_I2C  ?= I2C_DEV\(0\)
TEST_LPS331AP_ADDR ?= 92

CFLAGS += -DTEST_LPS331AP_I2C=$(TEST_LPS331AP_I2C)
CFLAGS += -DTEST_LPS331AP_ADDR=$(TEST_LPS331AP_ADDR)

include $(RIOTBASE)/Makefile.include
