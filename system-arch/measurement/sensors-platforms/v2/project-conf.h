#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__
/*-------------------------------------------------*/
#define DeviceFamily_PARENT_CC13X0_CC26X0      1
/*-------------------------------------------------*/
#define DeviceFamily_ID_CC26X0                 2
#define WITH_ADVANCED_SENSORS                           1
#define WITH_ENERGEST_MODULE                             0
/*-------------------------------------------------*/
#ifndef ADVANCED_SENSORS
    #define ADVANCED_SENSORS    WITH_ADVANCED_SENSORS
#endif
/*-------------------------------------------------*/
#ifndef ENERGEST_MODULE
    #define ENERGEST_MODULE     WITH_ENERGEST_MODULE
#endif
/*-------------------------------------------------*/
#define UDP_SERVER_PORT 1234
#define UDP_CLIENT_PORT 4321
/*-------------------------------------------------*/
#define LOG_MODULE "F4D"
#define LOG_LEVEL LOG_LEVEL_NONE
/*-------------------------------------------------*/
#define BASE_SENSORS_PAYLOAD_BUFFER_SIZE 19
#define ADVANCED_SENSORS_PAYLOAD_BUFFER_SIZE 3
#define ENERGEST_PAYLOAD_BUFFER_SIZE 8
/*-------------------------------------------------*/
#define BYTES_PER_INT 4
#define BYTES_PER_UINT32 4
/*-------------------------------------------------*/
/* Processes Timers */
/*-------------------------------------------------*/
#define BASE_SENSORS_SLEEP_DURATION             (CLOCK_SECOND * 160)
#define SP_SINGLE_SENSOR_READING_PERIOD         (CLOCK_SECOND * 160)
#define ADVANCED_SENSORS_SLEEP_DURATION         (CLOCK_SECOND * 265)
//#define ADVANCED_SENSORS_SLEEP_DURATION         (CLOCK_SECOND * 530)
#define UDP_PKT_RESEND_PERIOD                   (CLOCK_SECOND * 30)
#define WARMUP_PERIOD                           (CLOCK_SECOND * 15)
#define DELAY_PERIOD                           (CLOCK_SECOND * 5)

#define SENSOR_READING_RANDOM                   (CLOCK_SECOND << 4)
#define OPT_HANDLE_AQUIRE_TIMER                 (CLOCK_SECOND * 2)
#define OPTU123_READING_RANDOM                  (CLOCK_SECOND << 4)
/*-------------------------------------------------*/
/* Network Configuration */
/*-------------------------------------------------*/
#define UIP_CONF_BUFFER_SIZE 200                  // Set buffer size
// #define IEEE802154_CONF_PANID 0x9998             // Set PAN ID for IEEE 802.15.4
#define IEEE802154_CONF_PANID 0xF4D0               // Set PAN ID for IEEE 802.15.4
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 4        // Set default length for TSCH schedule
#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &unicast_per_neighbor_rpl_storing, &default_common } // Orchestra rules configuration
/*-------------------------------------------------*/
/* 6LoWPAN and TSCH Configuration */
/*-------------------------------------------------*/
#define SICSLOWPAN_CONF_FRAG 0                     // Disable 6LoWPAN fragmentation
#define TSCH_CONF_AUTOSTART 0                      // Disable TSCH autostart
#define TSCH_LOG_CONF_PER_SLOT 0                   // Disable TSCH logging per slot
/*-------------------------------------------------*/
/* Logging Configuration */
/*-------------------------------------------------*/
#define LOG_CONF_LEVEL_RPL LOG_LEVEL_NONE          // Disable RPL logging
#define LOG_CONF_LEVEL_TCPIP LOG_LEVEL_NONE        // Disable TCP/IP logging
#define LOG_CONF_LEVEL_IPV6 LOG_LEVEL_NONE         // Disable IPv6 logging
#define LOG_CONF_LEVEL_6LOWPAN LOG_LEVEL_NONE      // Disable 6LoWPAN logging
#define LOG_CONF_LEVEL_MAC LOG_LEVEL_NONE          // Disable MAC layer logging
/*-------------------------------------------------*/
/* Miscellaneous Configuration */
/*-------------------------------------------------*/
#define WATCHDOG_CONF_DISABLE 1                  // Disable the watchdog
#define RF_BLE_CONF_ENABLED 0                      // Disable BLE
#define RF_CONF_MODE RF_MODE_2_4_GHZ               // Set RF mode to 2.4 GHz
#define CCXXWARE_CONF_ROM_BOOTLOADER_ENABLE 0      // Disable ROM bootloader
#define ENERGEST_CONF_ON                0                      // Enable energest module


#endif /* __PROJECT_CONF_H__ */


