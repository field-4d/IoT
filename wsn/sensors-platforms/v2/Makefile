CONTIKI_PROJECT = sensorsPlatform

CONTIKI=../../../../contiki-ng

DEBUG = 1

MAKE_MAC = MAKE_MAC_TSCH
MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
MAKE_NET = MAKE_NET_IPV6

TARGET=simplelink
BOARD=imf4d/cc2650

#ifeq ($(ENERGEST_CONF_ON),1)
#PROJECTDIRS += .
#PROJECT_SOURCEFILES += energest_handler.c
#endif

CORE_SDK=../../coresdk_cc13xx_cc26xx

LDFLAGS += -L$(CORE_SDK)/lib

#CFLAGS += -I$(CONTIKI)/arch/cpu/cc26x0-cc13x0/
CFLAGS += -I$(CORE_SDK)/include
CFLAGS += -I$(CONTIKI)/arch/cpu/cc26x0-cc13x0/lib/cc26xxware
CFLAGS += -g
CFLAGS += -w






MODULES += os/services/orchestra
MODULES += arch/platform/simplelink/cc26x0-cc13x0/imf4d

MODULES_REL += $(TARGET)

all: $(CONTIKI_PROJECT)



include $(CONTIKI)/Makefile.include




#CFLAGS += -Os  # Optimize for size

# Include the PWM module
#MODULES += arch/platform/simplelink/cc13xx-cc26xx/driverlib
#MODULES += arch/cpu/simplelink-cc13xx-cc26xx


#CFLAGS += -I$(CONTIKI)/arch/cpu/simplelink-cc13xx-cc26xx
#CFLAGS += -I$(CONTIKI)/arch/platform/simplelink/cc26x0-cc13x0
#CFLAGS += -I$(CONTIKI)/core/sys
#CFLAGS += -I$(CONTIKI)/core/net
#CFLAGS += -I$(CONTIKI)/core/dev
#CFLAGS += -I$(CONTIKI)/core/lib
#CFLAGS += -I$(CONTIKI)/arch/cpu/simplelink-cc13xx-cc26xx
# Include path to the TI Drivers
#CFLAGS += -I$(CONTIKI_NG)/arch/platform/simplelink/cc13xx-cc26xx/driverlib
#CFLAGS += -I$(CONTIKI_NG)/arch/platform/simplelink/cc13xx-cc26xx
#CFLAGS += -I$(CONTIKI_NG)/arch/platform/simplelink/cc13xx-cc26xx/$(TARGET)
#CFLAGS += -DNRF_CONF_STACK_CONF_SIZE=2048


#-include $(CONTIKI)/Makefile.identify-target
