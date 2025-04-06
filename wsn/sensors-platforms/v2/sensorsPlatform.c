/*---------------------------------------------------------------------------*/
/*                                                 sensorsPlatform.c                                                 */
/*---------------------------------------------------------------------------*/
#include "sensorsPlatform.h"
#include "dev/button-hal.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "mac/tsch/tsch.h"
#include "routing/routing.h"
#include "sys/log.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
/*-------------------------------------------------*/
#if ADVANCED_SENSORS
    static uint8_t advanced_cycle_count = 0;
    static int temp_value = 0;
#endif
/*-------------------------------------------------*/
static process_event_t event_to_post;
/*-------------------------------------------------*/
PROCESS(onboard_sensors_controller, "Onboard Sensors process.");
PROCESS(udp_handler_process, "UDP Sending AND Registration Process");
/*-------------------------------------------------*/
AUTOSTART_PROCESSES( &onboard_sensors_controller,  &udp_handler_process);
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
PROCESS_THREAD(udp_handler_process, ev, data)
{
  PROCESS_BEGIN();

  EVENT_BASE_PACKET_READY = process_alloc_event();
  if(EVENT_BASE_PACKET_READY == PROCESS_EVENT_NONE) {
      printf("Failed to allocate EVENT_BASE_PACKET_READY\n");
      PROCESS_EXIT();
  }

  EVENT_ADVANCED_PACKET_READY = process_alloc_event();
  if(EVENT_ADVANCED_PACKET_READY == PROCESS_EVENT_NONE) {
      printf("Failed to allocate EVENT_ADVANCED_PACKET_READY\n");
      PROCESS_EXIT();
  }

  if (!is_registered) {
      simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);
      is_registered = true;
  }

  while(1) {
      ack_received = false; // Reset ACK flag
      PROCESS_WAIT_EVENT_UNTIL(ev == EVENT_BASE_PACKET_READY || ev == EVENT_ADVANCED_PACKET_READY);

      if(ev == EVENT_BASE_PACKET_READY) {
          // Handle base payload
          if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
              simple_udp_sendto(&udp_conn, bytes_base_payload, sizeof(bytes_base_payload), &dest_ipaddr);
          }
          ETIMER_SET_AND_WAIT(&resend_timer, UDP_PKT_RESEND_PERIOD);
          if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
              simple_udp_sendto(&udp_conn, bytes_base_payload, sizeof(bytes_base_payload), &dest_ipaddr);
          }

      } else {
          // This must be EVENT_ADVANCED_PACKET_READY
#if ADVANCED_SENSORS
          if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
              simple_udp_sendto(&udp_conn, bytes_advanced_payload, sizeof(bytes_advanced_payload), &dest_ipaddr);
          }
          ETIMER_SET_AND_WAIT(&resend_timer, UDP_PKT_RESEND_PERIOD);
          if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
              simple_udp_sendto(&udp_conn, bytes_advanced_payload, sizeof(bytes_advanced_payload), &dest_ipaddr);
          }
#endif
      }

#if ENERGEST_MODULE
       simple_udp_sendto(&udp_conn, bytes_energest_payload, sizeof(bytes_energest_payload), &dest_ipaddr);
#endif
  }

  PROCESS_END();
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
PROCESS_THREAD(onboard_sensors_controller, ev, data)
{
    PROCESS_BEGIN();

    NETSTACK_MAC.on();
    tsch_schedule_create_minimal();
    orchestra_init();
    SENSORS_ACTIVATE(batmon_sensor);

    etimer_set(&sensors_reading_timer, BASE_SENSORS_SLEEP_DURATION);
    while(1) {
        PROCESS_YIELD();

        /* Handle button presses at all times (not just during waiting) */
        if (ev == button_hal_press_event) {
            button_hal_button_t* btn = (button_hal_button_t*)data;
            if (btn->unique_id == PING_KEY) {
                int rssi = sicslowpan_get_last_rssi();
                printf("\n\rRSSI = %d", rssi);
                if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
                    simple_udp_sendto(&udp_conn, ping, sizeof(ping), &dest_ipaddr);
                    printf("\n\rping: %d", (int)sizeof(ping));
                }
            }
        }

        if(ev == PROCESS_EVENT_TIMER && data == &sensors_reading_timer) {
            /* BMP390 U18 */
            SENSORS_ACTIVATE(bmp_390_u18_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = bmp_390_u18_sensor.value(BMP_390_U18_SENSOR_TYPE_TEMP);
            payloadBaseBuffer.bmp_390_u18_temperature_int =
              (temporary_value != BMP_390_U18_READING_ERROR) ? temporary_value : 999;

            temporary_value = bmp_390_u18_sensor.value(BMP_390_U18_SENSOR_TYPE_PRESS);
            payloadBaseBuffer.bmp_390_u18_pressure_int =
              (temporary_value != BMP_390_U18_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(bmp_390_u18_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* BMP390 U19 */
            SENSORS_ACTIVATE(bmp_390_u19_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = bmp_390_u19_sensor.value(BMP_390_U19_SENSOR_TYPE_PRESS);
            payloadBaseBuffer.bmp_390_u19_pressure_int =
              (temporary_value != BMP_390_U19_READING_ERROR) ? temporary_value : 999;

            temporary_value = bmp_390_u19_sensor.value(BMP_390_U19_SENSOR_TYPE_TEMP);
            payloadBaseBuffer.bmp_390_u19_temperature_int =
              (temporary_value != BMP_390_U19_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(bmp_390_u19_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* OPT3001 U1 */
            SENSORS_ACTIVATE(opt_3001_sensor_u1);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = opt_3001_sensor_u1.value(0);
            payloadBaseBuffer.opt_3001_u1_light_intensity_int =
              (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(opt_3001_sensor_u1);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* OPT3001 U2 */
            SENSORS_ACTIVATE(opt_3001_sensor_u2);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = opt_3001_sensor_u2.value(0);
            payloadBaseBuffer.opt_3001_u2_light_intensity_int =
              (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(opt_3001_sensor_u2);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* OPT3001 U3 */
            SENSORS_ACTIVATE(opt_3001_sensor_u3);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = opt_3001_sensor_u3.value(0);
            payloadBaseBuffer.opt_3001_u3_light_intensity_int =
              (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(opt_3001_sensor_u3);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* OPT3001 U4 */
            SENSORS_ACTIVATE(opt_3001_sensor_u4);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = opt_3001_sensor_u4.value(0);
            payloadBaseBuffer.opt_3001_u4_light_intensity_int =
              (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(opt_3001_sensor_u4);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* OPT3001 U5 */
            SENSORS_ACTIVATE(opt_3001_sensor_u5);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = opt_3001_sensor_u5.value(0);
            payloadBaseBuffer.opt_3001_u5_light_intensity_int =
              (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(opt_3001_sensor_u5);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* HDC2010 U13 */
            SENSORS_ACTIVATE(hdc_2010_u13_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            temporary_value = hdc_2010_u13_sensor.value(HDC_2010_U13_SENSOR_TYPE_TEMP);
            payloadBaseBuffer.hdc_2010_u13_temperature_int =
              (temporary_value != HDC_2010_U13_READING_ERROR) ? temporary_value : 999;

            temporary_value = hdc_2010_u13_sensor.value(HDC_2010_U13_SENSOR_TYPE_HUMID);
            payloadBaseBuffer.hdc_2010_u13_humidity_int =
              (temporary_value != HDC_2010_U13_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(hdc_2010_u13_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* HDC2010 U16 */
            SENSORS_ACTIVATE(hdc_2010_u16_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = hdc_2010_u16_sensor.value(HDC_2010_U16_SENSOR_TYPE_TEMP);
            payloadBaseBuffer.hdc_2010_u16_temperature_int =
              (temporary_value != HDC_2010_U16_READING_ERROR) ? temporary_value : 999;

            temporary_value = hdc_2010_u16_sensor.value(HDC_2010_U16_SENSOR_TYPE_HUMID);
            payloadBaseBuffer.hdc_2010_u16_humidity_int =
              (temporary_value != HDC_2010_U16_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(hdc_2010_u16_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();

            /* HDC2010 U17 */
            SENSORS_ACTIVATE(hdc_2010_u17_sensor);
            etimer_set(&standby_etimer, 2 * CLOCK_SECOND);
            WAIT_FOR_STANDBY_TIMER();
            temporary_value = hdc_2010_u17_sensor.value(HDC_2010_U17_SENSOR_TYPE_TEMP);
            payloadBaseBuffer.hdc_2010_u17_temperature_int =
              (temporary_value != HDC_2010_U17_READING_ERROR) ? temporary_value : 999;

            temporary_value = hdc_2010_u17_sensor.value(HDC_2010_U17_SENSOR_TYPE_HUMID);
            payloadBaseBuffer.hdc_2010_u17_humidity_int =
              (temporary_value != HDC_2010_U17_READING_ERROR) ? temporary_value : 999;

            SENSORS_DEACTIVATE(hdc_2010_u17_sensor);


           base_pkt_counter++;

           payloadBaseBuffer.rssi = sicslowpan_get_last_rssi();
           payloadBaseBuffer.package_number = base_pkt_counter;

           /* CC26xx AON battery monitor. */
            temporary_value = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
            payloadBaseBuffer.batmon_battery_voltage_int = (temporary_value * 125) >> 5;
//            SENSORS_DEACTIVATE(batmon_sensor);

           convertPayloadToBytes(&payloadBaseBuffer, bytes_base_payload, sizeof(PayloadBaseBuffer) / sizeof(int));


#if ADVANCED_SENSORS
                       /* Trigger advanced sensors logic every third base cycle */
                       advanced_cycle_count++;
                       if(advanced_cycle_count == 3) {
//                           advanced_cycle_count = 0;

                           if(!GPIO_setEnable(CC2650IMF4D_DCDC_EN, GPIO_ON)) {
                               printf("DC/DC failed to turn on.");
                           }

                           if(!GPIO_setEnable(CC2650IMF4D_T6713_EN, GPIO_ON)) {
                               printf("T6713 failed to turn on.");
                           }

                           /* Warm-up period before CO2 reading */
                           etimer_set(&standby_etimer, WARMUP_PERIOD);
                           WAIT_FOR_STANDBY_TIMER();

                           payloadAdvancedBuffer.t6713_co2_value = t6713_write_and_read_ppm();

                           if(!GPIO_setEnable(CC2650IMF4D_T6713_EN, GPIO_OFF)) {
                               printf("T6713 failed to turn off.");
                           }

                           /* Another warm-up period before airflow reading */
                           etimer_set(&standby_etimer, DELAY_PERIOD);
                           WAIT_FOR_STANDBY_TIMER();

                           temp_value = read_from_f662();
                           payloadAdvancedBuffer.f662_airflow_value = (temp_value == -1) ? 15 : temp_value;

                           if(!GPIO_setEnable(CC2650IMF4D_DCDC_EN, GPIO_OFF)) {
                               printf("DC/DC failed to turn off.");
                           }

                           /* Prepare advanced packet */
                           advanced_pkt_counter++;
                           payloadAdvancedBuffer.package_number = advanced_pkt_counter;

                           convertPayloadToBytes(&payloadAdvancedBuffer,\
                                                 bytes_advanced_payload, sizeof(PayloadAdvancedBuffer) / sizeof(int));



                       }
#endif

#if ENERGEST_MODULE
                       uint32_t *energest_buffer = energy_stats(true, period_count);
                       convertEnergstPayloadToBytes(energest_buffer, bytes_energest_payload, ENERGEST_PAYLOAD_BUFFER_SIZE);
                       bzero(energest_buffer, sizeof(*energest_buffer) * ENERGEST_PAYLOAD_BUFFER_SIZE);
                       period_count++;
#endif



#if ADVANCED_SENSORS
                       if(advanced_cycle_count == 3) {
                         event_to_post = EVENT_ADVANCED_PACKET_READY;
                         advanced_cycle_count = 0; // reset the counter
                       } else {
                         event_to_post = EVENT_BASE_PACKET_READY;
                       }
#else
                       event_to_post = EVENT_BASE_PACKET_READY;
#endif
                       process_post(&udp_handler_process, event_to_post, NULL);

                       /* Reset main timer for the next reading cycle */

                       etimer_reset(&sensors_reading_timer);
                       bzero(&payloadBaseBuffer, sizeof(payloadBaseBuffer));
#if ADVANCED_SENSORS
                       bzero(&payloadAdvancedBuffer, sizeof(payloadAdvancedBuffer));
#endif
                   }
               }

               PROCESS_END();
           }
