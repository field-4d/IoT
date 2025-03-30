/*---------------------------------------------------------------------------*/
/*                                                 sensorsPlatform.h                                              */
/*---------------------------------------------------------------------------*/
#ifndef SENSORSPLATFORM_H
#define SENSORSPLATFORM_H

#include "contiki.h"
#include "project-conf.h"
#include "dev/gpio-hal.h"
#include "arch/cpu/cc26x0-cc13x0/ti-lib.h"
#include "net/ipv6/simple-udp.h"
#include "lib/sensors.h"
#include "batmon-sensor.h"
#include "board-peripherals.h"
#include <Board.h>
#include "random.h"

#if ADVANCED_SENSORS
#include "f662-sensor.h"
#endif
/*-------------------------------------------------*/
static bool ack_received = false;
static bool is_registered = false;
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
typedef struct {
    int bmp_390_u18_pressure_int;
    int bmp_390_u18_temperature_int;
    int bmp_390_u19_pressure_int;
    int bmp_390_u19_temperature_int;

    int hdc_2010_u13_temperature_int;
    int hdc_2010_u13_humidity_int;
    int hdc_2010_u16_temperature_int;
    int hdc_2010_u16_humidity_int;
    int hdc_2010_u17_temperature_int;
    int hdc_2010_u17_humidity_int;

    int opt_3001_u1_light_intensity_int;
    int opt_3001_u2_light_intensity_int;
    int opt_3001_u3_light_intensity_int;
    int opt_3001_u4_light_intensity_int;
    int opt_3001_u5_light_intensity_int;

    int batmon_temperature_int;
    int batmon_battery_voltage_int;

    int package_number;
    int rssi;
} PayloadBaseBuffer;
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
typedef struct {
    int t6713_co2_value;      // payload_advanced_buffer[0]
    int f662_airflow_value;   // payload_advanced_buffer[1]
    int package_number;       // payload_advanced_buffer[2]
} PayloadAdvancedBuffer;
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
static uint8_t bytes_base_payload[BASE_SENSORS_PAYLOAD_BUFFER_SIZE * BYTES_PER_INT];
static uint8_t bytes_advanced_payload[ADVANCED_SENSORS_PAYLOAD_BUFFER_SIZE * BYTES_PER_INT];
static uint8_t bytes_energest_payload[ENERGEST_PAYLOAD_BUFFER_SIZE * BYTES_PER_INT];
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
static struct etimer  sensors_reading_timer, resend_timer,standby_etimer;
static struct stimer delay_stimer;
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
PayloadBaseBuffer payloadBaseBuffer;
#if ADVANCED_SENSORS
PayloadAdvancedBuffer payloadAdvancedBuffer;
#endif
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
static struct simple_udp_connection udp_conn;
static uip_ipaddr_t dest_ipaddr;
#define PING_KEY             BUTTON_HAL_ID_KEY_PING
static   uint16_t ping[1]  =  { 16 };
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
static unsigned int period_count = 0;
static int send_count = 0;
static int base_pkt_counter = 0;
static int advanced_pkt_counter = 0;
static int temporary_value;
static int  current_sensor = 0;
/*-------------------------------------------------*/
/* Declare Custom Events */
/*-------------------------------------------------*/
static process_event_t EVENT_BASE_PACKET_READY;
static process_event_t EVENT_ADVANCED_PACKET_READY;
/*---------------------------------------------------------------------------------------*
 || Helper macro to handle waiting for standby_etimer while listening for button events      ||
 *---------------------------------------------------------------------------------------*/
#define WAIT_FOR_STANDBY_TIMER() do { \
    while(!etimer_expired(&standby_etimer)) { \
        PROCESS_WAIT_EVENT(); \
        if(ev == button_hal_press_event) { \
            button_hal_button_t* btn = (button_hal_button_t*)data; \
            if (btn->unique_id == PING_KEY) { \
                int rssi = sicslowpan_get_last_rssi(); \
                printf("\n\rRSSI = %d", rssi); \
                if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) { \
                    simple_udp_sendto(&udp_conn, ping, sizeof(ping), &dest_ipaddr); \
                    printf("\n\rping: %d", (int)sizeof(ping)); \
                } \
            } \
        } \
    } \
} while(0)
/*-------------------------------------------------*/
#define ETIMER_SET_AND_WAIT(etimer, interval) \
  do { \
    etimer_set(etimer, interval); \
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == etimer); \
  } while(0)
/*-------------------------------------------------*/
// Define an enum to represent GPIO states
typedef enum {
  GPIO_OFF = 0,
  GPIO_ON = 1
} gpio_state_t;
/*-------------------------------------------------*/
bool GPIO_setEnable(uint32_t pin, gpio_state_t state) {
    // Write the desired state to the GPIO pin
    GPIO_writeDio(pin, state);

    // Verify the written state by reading back the GPIO pin
    if (GPIO_readDio(pin) == state) {
        return true; // Success: state matches
    }
    return false; // Failure: state mismatch
}
/*-------------------------------------------------*/
// Define a struct to hold the GPIO states
/*-------------------------------------------------*/
typedef struct {
  gpio_state_t dcdc_en;
  gpio_state_t ztp315_en;
  gpio_state_t led_pwm;
  gpio_state_t t6713_pwm;
  gpio_state_t f662_en;
  gpio_state_t accel_en;
  gpio_state_t adc_on;
} gpio_states_t;
/*-------------------------------------------------*/
// Function to read GPIO states
/*-------------------------------------------------*/
void read_gpio_states(gpio_states_t *states) {
  states->dcdc_en = GPIO_readDio(CC2650IMF4D_DCDC_EN) ? GPIO_ON : GPIO_OFF;
  states->ztp315_en = GPIO_readDio(CC2650IMF4D_ZTP315_EN) ? GPIO_ON : GPIO_OFF;
//  states->led_pwm = GPIO_readDio(LED_PWM) ? GPIO_OFF : GPIO_ON;
  states->led_pwm = GPIO_readDio(CC2650IMF4D_LED_PWM) ? GPIO_ON : GPIO_OFF;
  states->t6713_pwm = GPIO_readDio(CC2650IMF4D_T6713_PWM) ? GPIO_OFF : GPIO_ON;
  states->f662_en = GPIO_readDio(CC2650IMF4D_F662_EN) ? GPIO_ON : GPIO_OFF;
  states->accel_en = GPIO_readDio(CC2650IMF4D_ACCEL_EN) ? GPIO_ON : GPIO_OFF;
  states->adc_on = GPIO_readDio(CC2650IMF4D_ADC_ON) ? GPIO_ON : GPIO_OFF;
}
/*-------------------------------------------------*/
// Function to turn all GPIO pins off
/*-------------------------------------------------*/
void turn_all_gpio_off(void) {
  GPIO_writeDio(CC2650IMF4D_DCDC_EN, 0);
  GPIO_writeDio(CC2650IMF4D_ZTP315_EN, 0);
  GPIO_writeDio(CC2650IMF4D_LED_PWM, 0); // LED_PWM logic reversed
  GPIO_writeDio(CC2650IMF4D_T6713_PWM, 1); // T6713_PWM logic reversed
  GPIO_writeDio(CC2650IMF4D_F662_EN, 0);
  GPIO_writeDio(CC2650IMF4D_ACCEL_EN, 0);
  GPIO_writeDio(CC2650IMF4D_ADC_ON, 0);
}
/*-------------------------------------------------*/
// Function to turn all GPIO pins on
/*-------------------------------------------------*/
void turn_all_gpio_on(void) {
  GPIO_writeDio(CC2650IMF4D_DCDC_EN, 1);
  GPIO_writeDio(CC2650IMF4D_ZTP315_EN, 1);
  GPIO_writeDio(CC2650IMF4D_LED_PWM, 1); // LED_PWM logic reversed
  GPIO_writeDio(CC2650IMF4D_T6713_PWM, 0); // T6713_PWM logic reversed
  GPIO_writeDio(CC2650IMF4D_F662_EN, 1);
  GPIO_writeDio(CC2650IMF4D_ACCEL_EN, 1);
  GPIO_writeDio(CC2650IMF4D_ADC_ON, 1);
}
gpio_states_t current_states;
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
static int are_other_sensors_active() {
    const struct sensors_sensor *s;
    for (s = sensors_first(); s != NULL; s = sensors_next(s)) {
        if (s->status(SENSORS_ACTIVE)) {
            return 1; // An active sensor was found
        }
    }
    return 0; // No active sensors
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
void deactivate_all_active_sensors() {
  const struct sensors_sensor *s;
  for (s = sensors_first(); s != NULL; s = sensors_next(s)) {
      SENSORS_DEACTIVATE(*s); // Deactivate the sensor using macro
  }
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
void activate_all_sensors() {
  const struct sensors_sensor *s;
  for (s = sensors_first(); s != NULL; s = sensors_next(s)) {
      SENSORS_ACTIVATE(*s); // Activate the sensor using macro
  }
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
void int_to_bytes(int value, uint8_t *bytes) {
    // Assuming the system is little-endian.
    // Otherwise, byte order needs to be reversed.
    bytes[0] = (uint8_t)(value & 0xFF);
    bytes[1] = (uint8_t)((value >> 8) & 0xFF);
    bytes[2] = (uint8_t)((value >> 16) & 0xFF);
    bytes[3] = (uint8_t)((value >> 24) & 0xFF);
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
bool compareArrays(uint8_t *array1, uint8_t *array2, size_t start, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (array1[i] != array2[start + i]) {
            return false;
        }
    }
    return true;
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
void convertPayloadToBytes(PayloadBaseBuffer *payloadBaseBuffer, uint8_t *bytes_base_payload, int payload_buffer_size) {
    int byte_index = 0;
    int *payload = (int*)payloadBaseBuffer; // Cast payloadBaseBuffer to int pointer
    for(int i = 0; i < payload_buffer_size; ++i) {
        int_to_bytes(payload[i], &bytes_base_payload[byte_index]);
        byte_index += BYTES_PER_INT;
    }
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
void uint32_to_bytes(uint32_t num, uint8_t *bytes) {
    bytes[0] = (uint8_t)(num >> 24);
    bytes[1] = (uint8_t)(num >> 16);
    bytes[2] = (uint8_t)(num >> 8);
    bytes[3] = (uint8_t)num;
}
/*-------------------------------------------------*/
//
/*-------------------------------------------------*/
void convertEnergstPayloadToBytes(uint32_t *payload, uint8_t *bytes_base_payload, int payload_buffer_size) {
    int byte_index = 0;
    for (int i = 0; i < payload_buffer_size; ++i) {
        uint32_to_bytes(payload[i], &bytes_base_payload[byte_index]);
        byte_index += BYTES_PER_UINT32;
    }
}

#endif /* SENSORSPLATFORM_H */
