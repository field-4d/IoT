/****************************************************************************/
/*                          sensorsPlatform.c                             */
/****************************************************************************/
#include "contiki.h"
#include "sensorsPlatform.h"
#include "sys/ctimer.h"
#include "sys/etimer.h" 
#include "net/netstack.h"
#include "mac/tsch/tsch.h"
#include "dev/button-hal.h"
#include "routing/routing.h"
#include "sys/log.h"
#include "lib/random.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
/*---------------------------------------------------------------------------------------*/
/* Core process declarations */
PROCESS(network_handler_process, "Network Handler Process");
PROCESS(basic_package_process,   "Basic Package Process");
#if ADVANCED_SENSORS
PROCESS(advanced_sensors_process, "Advanced Package Process");
#endif
/*---------------------------------------------------------------------------------------*/
/* Autostart only those processes that are enabled */
/*---------------------------------------------------------------------------------------*/
AUTOSTART_PROCESSES(
  &network_handler_process,
  &basic_package_process
#if ADVANCED_SENSORS
  , &advanced_sensors_process
#endif
);
/*---------------------------------------------------------------------------------------*/
PROCESS_THREAD(network_handler_process, ev, data)
{
  PROCESS_BEGIN();

  /* MAC Initialization */
  NETSTACK_MAC.on();
  /* TSCH Initialization */
  tsch_schedule_create_minimal();
  /* Orchestra Initialization */
  orchestra_init();

  /* Register UDP if not already done */
  if(!is_registered) {
    simple_udp_register(&udp_conn,
                        UDP_CLIENT_PORT,
                        NULL,
                        UDP_SERVER_PORT,
                        NULL);
    is_registered = true;
  }

  while (1)
  {
        PROCESS_WAIT_EVENT_UNTIL(ev == button_hal_press_event);
            button_hal_button_t* btn = (button_hal_button_t*)data;
            if (btn->unique_id == PING_KEY) {
                if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
                    simple_udp_sendto(&udp_conn, ping, sizeof(ping), &dest_ipaddr);
                    printf("\n\rping: %d", (int)sizeof(ping));
                }
            }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------------------*/
PROCESS_THREAD(basic_package_process, ev, data)
{
    static struct etimer sched_timer, delay_timer;
    /* Calculate your total read overhead: sum of all delays */
    const clock_time_t OVERHEAD = (25 * CLOCK_SECOND); 
    /* Schedule sends with jitter around 3 min */
    const clock_time_t PERIOD_BASE = (180 * CLOCK_SECOND);
    const clock_time_t JITTER_MAX = (10 * CLOCK_SECOND);  // ±10 seconds of jitter
    
    PROCESS_BEGIN();

    /* Add random jitter to initial scheduling to avoid synchronization */
    clock_time_t initial_delay = PERIOD_BASE - OVERHEAD + (random_rand() % JITTER_MAX) - (JITTER_MAX/2);
    etimer_set(&sched_timer, initial_delay);

    while(1) {
        /* 1) Sleep until it's time to read & send */
        PROCESS_WAIT_EVENT_UNTIL(ev==PROCESS_EVENT_TIMER && data==&sched_timer);


        /*---------------------------------------------------------------------------------------*/

        /* ZTP-315 */
        SENSORS_ACTIVATE(ztp_315_sensor);

        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));

        temporary_value = ztp_315_sensor.value(ZTP315_SENSOR_TYPE_AMBIENT_LINEAR);
        payloadBaseBuffer.ztp_315_ambient_temperature_int =
             (temporary_value != ZTP315_SENSOR_READING_ERROR) ? temporary_value : 999;

        temporary_value = ztp_315_sensor.value(ZTP315_SENSOR_TYPE_OBJECT);
        payloadBaseBuffer.ztp_315_object_temperature_int =
             (temporary_value != ZTP315_SENSOR_READING_ERROR) ? temporary_value : 999;


        SENSORS_DEACTIVATE(ztp_315_sensor);
        /*---------------------------------------------------------------------------------------*/
        /* BMP390 U18 */
        SENSORS_ACTIVATE(bmp_390_u18_sensor);
        /*---------------------------------------------------------------------------------------*/
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        temporary_value = bmp_390_u18_sensor.value(BMP_390_U18_SENSOR_TYPE_TEMP);
        payloadBaseBuffer.bmp_390_u18_temperature_int =
          (temporary_value != BMP_390_U18_READING_ERROR) ? temporary_value : 999;

        temporary_value = bmp_390_u18_sensor.value(BMP_390_U18_SENSOR_TYPE_PRESS);
        payloadBaseBuffer.bmp_390_u18_pressure_int =
          (temporary_value != BMP_390_U18_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(bmp_390_u18_sensor);
        /*---------------------------------------------------------------------------------------*/
        /* BMP390 U19 */
        SENSORS_ACTIVATE(bmp_390_u19_sensor);
        /*---------------------------------------------------------------------------------------*/
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        
        temporary_value = bmp_390_u19_sensor.value(BMP_390_U19_SENSOR_TYPE_PRESS);
        payloadBaseBuffer.bmp_390_u19_pressure_int =
          (temporary_value != BMP_390_U19_READING_ERROR) ? temporary_value : 999;

        temporary_value = bmp_390_u19_sensor.value(BMP_390_U19_SENSOR_TYPE_TEMP);
        payloadBaseBuffer.bmp_390_u19_temperature_int =
          (temporary_value != BMP_390_U19_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(bmp_390_u19_sensor);
        /*---------------------------------------------------------------------------------------*/
        /* OPT3001 U1 */
        SENSORS_ACTIVATE(opt_3001_sensor_u1);
        /*---------------------------------------------------------------------------------------*/
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));

        temporary_value = opt_3001_sensor_u1.value(0);
        payloadBaseBuffer.opt_3001_u1_light_intensity_int =
          (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(opt_3001_sensor_u1);
        /*---------------------------------------------------------------------------------------*/
        /* OPT3001 U2 */
        SENSORS_ACTIVATE(opt_3001_sensor_u2);
        /*---------------------------------------------------------------------------------------*/
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));

        temporary_value = opt_3001_sensor_u2.value(0);
        payloadBaseBuffer.opt_3001_u2_light_intensity_int =
          (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(opt_3001_sensor_u2);
        /*---------------------------------------------------------------------------------------*/
        /* OPT3001 U3 */
        SENSORS_ACTIVATE(opt_3001_sensor_u3);
        /*---------------------------------------------------------------------------------------*/
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        
        temporary_value = opt_3001_sensor_u3.value(0);
        payloadBaseBuffer.opt_3001_u3_light_intensity_int =
        (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;
        
        SENSORS_DEACTIVATE(opt_3001_sensor_u3);
        /*---------------------------------------------------------------------------------------*/
        /* OPT3001 U4 */
        SENSORS_ACTIVATE(opt_3001_sensor_u4);
        /*---------------------------------------------------------------------------------------*/
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));

        temporary_value = opt_3001_sensor_u4.value(0);
        payloadBaseBuffer.opt_3001_u4_light_intensity_int =
          (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(opt_3001_sensor_u4);
        /*---------------------------------------------------------------------------------------*/
        /* OPT3001 U5 */
        SENSORS_ACTIVATE(opt_3001_sensor_u5);
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        temporary_value = opt_3001_sensor_u5.value(0);
        payloadBaseBuffer.opt_3001_u5_light_intensity_int =
          (temporary_value != OPT_3001_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(opt_3001_sensor_u5);
        /*---------------------------------------------------------------------------------------*/
        /* HDC2010 U13 */
        SENSORS_ACTIVATE(hdc_2010_u13_sensor);
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));

        temporary_value = hdc_2010_u13_sensor.value(HDC_2010_U13_SENSOR_TYPE_TEMP);
        payloadBaseBuffer.hdc_2010_u13_temperature_int =
          (temporary_value != HDC_2010_U13_READING_ERROR) ? temporary_value : 999;

        temporary_value = hdc_2010_u13_sensor.value(HDC_2010_U13_SENSOR_TYPE_HUMID);
        payloadBaseBuffer.hdc_2010_u13_humidity_int =
          (temporary_value != HDC_2010_U13_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(hdc_2010_u13_sensor);
        
        /* HDC2010 U16 */
        SENSORS_ACTIVATE(hdc_2010_u16_sensor);

        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        temporary_value = hdc_2010_u16_sensor.value(HDC_2010_U16_SENSOR_TYPE_TEMP);
        payloadBaseBuffer.hdc_2010_u16_temperature_int =
          (temporary_value != HDC_2010_U16_READING_ERROR) ? temporary_value : 999;

        temporary_value = hdc_2010_u16_sensor.value(HDC_2010_U16_SENSOR_TYPE_HUMID);
        payloadBaseBuffer.hdc_2010_u16_humidity_int =
          (temporary_value != HDC_2010_U16_READING_ERROR) ? temporary_value : 999;

        SENSORS_DEACTIVATE(hdc_2010_u16_sensor);

        /* HDC2010 U17 */
        SENSORS_ACTIVATE(hdc_2010_u17_sensor);
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        temporary_value = hdc_2010_u17_sensor.value(HDC_2010_U17_SENSOR_TYPE_TEMP);
        payloadBaseBuffer.hdc_2010_u17_temperature_int =
        (temporary_value != HDC_2010_U17_READING_ERROR) ? temporary_value : 999;
        
        temporary_value = hdc_2010_u17_sensor.value(HDC_2010_U17_SENSOR_TYPE_HUMID);
        payloadBaseBuffer.hdc_2010_u17_humidity_int =
        (temporary_value != HDC_2010_U17_READING_ERROR) ? temporary_value : 999;
        
        SENSORS_DEACTIVATE(hdc_2010_u17_sensor);
        
        
        
        base_pkt_counter++;    
        payloadBaseBuffer.package_number = base_pkt_counter;
       /* CC26xx AON battery monitor. */
        SENSORS_ACTIVATE(batmon_sensor);
        
        etimer_set(&delay_timer, 2 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&delay_timer));
        
        temporary_value = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
        payloadBaseBuffer.batmon_battery_voltage_int = (temporary_value * 125) >> 5;
//                SENSORS_DEACTIVATE(batmon_sensor);

        convertPayloadToBytes(&payloadBaseBuffer,
                              bytes_base_payload,
                              sizeof(PayloadBaseBuffer) / sizeof(int));

        if(NETSTACK_ROUTING.node_is_reachable() &&
           NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
          simple_udp_sendto(&udp_conn,
                            bytes_base_payload,
                            sizeof(bytes_base_payload),
                            &dest_ipaddr);
          bzero(&payloadBaseBuffer, sizeof(payloadBaseBuffer));
        }

        /* Reschedule next cycle with jitter */
        clock_time_t next_period = PERIOD_BASE - OVERHEAD + (random_rand() % JITTER_MAX) - (JITTER_MAX/2);
        etimer_set(&sched_timer, next_period);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------------------*/
#if ADVANCED_SENSORS
PROCESS_THREAD(advanced_sensors_process, ev, data)
{
  static struct etimer adv_sched_timer, standby_timer;
  const clock_time_t OVERHEAD = WARMUP_PERIOD + DELAY_PERIOD + (2 * CLOCK_SECOND);
  const clock_time_t PERIOD   = (540 * CLOCK_SECOND) - OVERHEAD;
  int temp_value;

  PROCESS_BEGIN();

  etimer_set(&adv_sched_timer, PERIOD);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &adv_sched_timer);

    GPIO_setEnable(CC2650IMF4D_DCDC_EN,  GPIO_ON);
    GPIO_setEnable(CC2650IMF4D_T6713_EN, GPIO_ON);

    etimer_set(&standby_timer, WARMUP_PERIOD);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &standby_timer);

    payloadAdvancedBuffer.t6713_co2_value = t6713_write_and_read_ppm();
    GPIO_setEnable(CC2650IMF4D_T6713_EN, GPIO_OFF);

    etimer_set(&standby_timer, DELAY_PERIOD);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &standby_timer);

    temp_value = read_from_f662();
    payloadAdvancedBuffer.f662_airflow_value = (temp_value < 0) ? 15 : temp_value;
    GPIO_setEnable(CC2650IMF4D_DCDC_EN, GPIO_OFF);

    advanced_pkt_counter++;
    payloadAdvancedBuffer.package_number = advanced_pkt_counter;
    convertPayloadToBytes(&payloadAdvancedBuffer,
                          bytes_advanced_payload,
                          sizeof(PayloadAdvancedBuffer) / sizeof(int));

    if(NETSTACK_ROUTING.node_is_reachable() &&
       NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
      simple_udp_sendto(&udp_conn,
                        bytes_advanced_payload,
                        sizeof(bytes_advanced_payload),
                        &dest_ipaddr);
    }

    etimer_set(&standby_timer, 2 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &standby_timer);

    bzero(&payloadAdvancedBuffer, sizeof(payloadAdvancedBuffer));

    etimer_set(&adv_sched_timer, PERIOD);
  }

  PROCESS_END();
}
#endif /* ADVANCED_SENSORS */
