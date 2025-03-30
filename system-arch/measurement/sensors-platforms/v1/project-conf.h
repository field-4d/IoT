
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* Logging */
#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_NONE
#define TSCH_LOG_CONF_PER_SLOT                     0

#define WATCHDOG_CONF_DISABLE  0
#define RF_BLE_CONF_ENABLED 0
#define RF_CONF_MODE RF_MODE_2_4_GHZ
#define SENSORTAG_CC2650_REV_1_2_0 0
#define CCXXWARE_CONF_ROM_BOOTLOADER_ENABLE 0
#define QUEUEBUF_CONF_ENABLED                   0


/* Enable printing of packet counters */
#define LINK_STATS_CONF_PACKET_COUNTERS          1

///* Application settings */
//#define APP_SEND_INTERVAL_SEC 10
//#define APP_WARM_UP_PERIOD_SEC 120

#define SICSLOWPAN_CONF_FRAG 0 /* No fragmentation */
#define UIP_CONF_BUFFER_SIZE 200


#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &unicast_per_neighbor_rpl_storing, &default_common }
#define IEEE802154_CONF_PANID 0x0059






#endif /* PROJECT_CONF_H_ */
