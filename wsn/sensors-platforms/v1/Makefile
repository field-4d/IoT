CONTIKI_PROJECT = sp_v1
CONTIKI=/Users/ifrachi/GitHub/f4d/contiki-ng
TARGET = cc26x0-cc13x0
BOARD =sensortag/cc2650



MODULES_REL += $(TARGET)

#CFLAGS += -g
CFLAGS += -w

MAKE_MAC = MAKE_MAC_TSCH
MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
MAKE_NET=MAKE_NET_IPV6
MODULES += os/services/orchestra


all: $(CONTIKI_PROJECT)





include $(CONTIKI)/Makefile.include
