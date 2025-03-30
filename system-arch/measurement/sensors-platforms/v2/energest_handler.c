#include "energest_handler.h"
#include <stdio.h>

static uint32_t last_cpu_time = 0;
static uint32_t last_lpm_time = 0;
static uint32_t last_deep_lpm_time = 0;
static uint32_t last_radio_tx_time = 0;
static uint32_t last_radio_rx_time = 0;
static rtimer_clock_t last_called_time = 0;
//static unsigned int period_count = 1;
static uint32_t energest_values_uint32[8];
uint32_t calculate_permil(rtimer_clock_t current, rtimer_clock_t total) {
  return 1000 * current / total;
}

void print_energest_info(const char *label, rtimer_clock_t current, rtimer_clock_t total) {
  printf("\r\n[INFO: Energest] %-11s: %10u/ %10u (%u permil)\n", label, current, total, calculate_permil(current, total));
}

uint32_t* energy_stats(bool print_output, unsigned int period_count) {
  rtimer_clock_t current_time = RTIMER_NOW();
  rtimer_clock_t period_time_ticks = current_time - last_called_time;
  uint32_t period_time_seconds = (uint32_t)(period_time_ticks / RTIMER_ARCH_SECOND);

  energest_flush(); // Update energest values
  // Store values in energest_values_uint32 directly as uint32_t
  energest_values_uint32[0] = energest_type_time(ENERGEST_TYPE_CPU) - last_cpu_time;
  energest_values_uint32[1] = energest_type_time(ENERGEST_TYPE_LPM) - last_lpm_time;
  energest_values_uint32[2] = energest_type_time(ENERGEST_TYPE_DEEP_LPM) - last_deep_lpm_time;
  energest_values_uint32[3] = energest_type_time(ENERGEST_TYPE_TRANSMIT) - last_radio_tx_time;
  energest_values_uint32[4] = energest_type_time(ENERGEST_TYPE_LISTEN) - last_radio_rx_time;
  energest_values_uint32[5] = (uint32_t)period_time_ticks;
  energest_values_uint32[6] = period_time_seconds;
  energest_values_uint32[7] = (uint32_t)period_count;
  if (print_output) {
    // Print the Energest values for the current period
    printf("\r\n");
    printf("[INFO: Energest] --- Period summary #%u (%u seconds)\n", energest_values_uint32[7], energest_values_uint32[6]);
    printf("\r\n[INFO: Energest] Total time  :    %u\n", energest_values_uint32[5]);
    print_energest_info("CPU", energest_values_uint32[0], energest_values_uint32[5]);
    print_energest_info("LPM", energest_values_uint32[1], energest_values_uint32[5]);
    print_energest_info("Deep LPM", energest_values_uint32[2], energest_values_uint32[5]);
    print_energest_info("Radio Tx", energest_values_uint32[3], energest_values_uint32[5]);
    print_energest_info("Radio Rx", energest_values_uint32[4], energest_values_uint32[5]);
  }

  last_called_time = current_time;
  last_cpu_time = energest_type_time(ENERGEST_TYPE_CPU);
  last_lpm_time = energest_type_time(ENERGEST_TYPE_LPM);
  last_deep_lpm_time = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
  last_radio_tx_time = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  last_radio_rx_time = energest_type_time(ENERGEST_TYPE_LISTEN);

  printf("\n");

  return energest_values_uint32;
}


//#include "energest_handler.h"
//#include <stdio.h>
////
////
////static uint32_t last_cpu_time = 0;
////static uint32_t last_lpm_time = 0;
////static uint32_t last_deep_lpm_time = 0;
////static uint32_t last_radio_tx_time = 0;
////static uint32_t last_radio_rx_time = 0;
////static rtimer_clock_t last_called_time = 0;
////
////
////uint32_t calculate_permil(rtimer_clock_t current, rtimer_clock_t total) {
////  return 1000 * current / total;
////}
////
////void print_energest_info(const char *label, rtimer_clock_t current, rtimer_clock_t total) {
////  printf("\r\n[INFO: Energest] %-11s: %10u/ %10u (%u permil)\n", label, current, total, calculate_permil(current, total));
////}
////
////energest_values_t* energy_stats(bool print_output, unsigned int period_count) {
////  static energest_values_t values;
////
////  rtimer_clock_t current_time = RTIMER_NOW();
////  rtimer_clock_t period_time_ticks = current_time - last_called_time;
////  uint32_t period_time_seconds = (uint32_t)(period_time_ticks / RTIMER_ARCH_SECOND);
////
////  energest_flush(); // Update energest values
////
////  values.cpu_time = energest_type_time(ENERGEST_TYPE_CPU) - last_cpu_time;
////  values.lpm_time = energest_type_time(ENERGEST_TYPE_LPM) - last_lpm_time;
////  values.deep_lpm_time = energest_type_time(ENERGEST_TYPE_DEEP_LPM) - last_deep_lpm_time;
////  values.radio_tx_time = energest_type_time(ENERGEST_TYPE_TRANSMIT) - last_radio_tx_time;
////  values.radio_rx_time = energest_type_time(ENERGEST_TYPE_LISTEN) - last_radio_rx_time;
////  values.period_time_ticks = period_time_ticks;
////  values.period_time_seconds = period_time_seconds;
////  values.period_count = period_count;
////
////  if (print_output) {
////    // Print the Energest values for the current period
////    printf("\r\n");
////    printf("[INFO: Energest] --- Period summary #%u (%u seconds)\n", values.period_count, values.period_time_seconds);
////    printf("\r\n[INFO: Energest] Total time  :    %u\n", values.period_time_ticks);
////    print_energest_info("CPU", values.cpu_time, values.period_time_ticks);
////    print_energest_info("LPM", values.lpm_time, values.period_time_ticks);
////    print_energest_info("Deep LPM", values.deep_lpm_time, values.period_time_ticks);
////    print_energest_info("Radio Tx", values.radio_tx_time, values.period_time_ticks);
////    print_energest_info("Radio Rx", values.radio_rx_time, values.period_time_ticks);
////  }
////
////  last_called_time = current_time;
////  last_cpu_time = energest_type_time(ENERGEST_TYPE_CPU);
////  last_lpm_time = energest_type_time(ENERGEST_TYPE_LPM);
////  last_deep_lpm_time = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
////  last_radio_tx_time = energest_type_time(ENERGEST_TYPE_TRANSMIT);
////  last_radio_rx_time = energest_type_time(ENERGEST_TYPE_LISTEN);
////
////  return &values;
////}
//
//
//
//static uint32_t last_cpu_time = 0;
//static uint32_t last_lpm_time = 0;
//static uint32_t last_deep_lpm_time = 0;
//static uint32_t last_radio_tx_time = 0;
//static uint32_t last_radio_rx_time = 0;
//static rtimer_clock_t last_called_time = 0;
////static unsigned int period_count = 1;
//static uint32_t energest_values_uint32[8];
//uint32_t calculate_permil(rtimer_clock_t current, rtimer_clock_t total) {
//  return 1000 * current / total;
//}
//
//void print_energest_info(const char *label, rtimer_clock_t current, rtimer_clock_t total) {
//  printf("\r\n[INFO: Energest] %-11s: %10u/ %10u (%u permil)\n", label, current, total, calculate_permil(current, total));
//}
//
//uint32_t* energy_stats(bool print_output, unsigned int period_count) {
//  rtimer_clock_t current_time = RTIMER_NOW();
//  rtimer_clock_t period_time_ticks = current_time - last_called_time;
//  uint32_t period_time_seconds = (uint32_t)(period_time_ticks / RTIMER_ARCH_SECOND);
//
//  energest_flush(); // Update energest values
//  // Store values in energest_values_uint32 directly as uint32_t
//  energest_values_uint32[0] = energest_type_time(ENERGEST_TYPE_CPU) - last_cpu_time;
//  energest_values_uint32[1] = energest_type_time(ENERGEST_TYPE_LPM) - last_lpm_time;
//  energest_values_uint32[2] = energest_type_time(ENERGEST_TYPE_DEEP_LPM) - last_deep_lpm_time;
//  energest_values_uint32[3] = energest_type_time(ENERGEST_TYPE_TRANSMIT) - last_radio_tx_time;
//  energest_values_uint32[4] = energest_type_time(ENERGEST_TYPE_LISTEN) - last_radio_rx_time;
//  energest_values_uint32[5] = (uint32_t)period_time_ticks;
//  energest_values_uint32[6] = period_time_seconds;
//  energest_values_uint32[7] = (uint32_t)period_count;
//  if (print_output) {
//    // Print the Energest values for the current period
//    printf("\r\n");
//    printf("[INFO: Energest] --- Period summary #%u (%u seconds)\n", energest_values_uint32[7], energest_values_uint32[6]);
//    printf("\r\n[INFO: Energest] Total time  :    %u\n", energest_values_uint32[5]);
//    print_energest_info("CPU", energest_values_uint32[0], energest_values_uint32[5]);
//    print_energest_info("LPM", energest_values_uint32[1], energest_values_uint32[5]);
//    print_energest_info("Deep LPM", energest_values_uint32[2], energest_values_uint32[5]);
//    print_energest_info("Radio Tx", energest_values_uint32[3], energest_values_uint32[5]);
//    print_energest_info("Radio Rx", energest_values_uint32[4], energest_values_uint32[5]);
//  }
//  last_called_time = current_time;
//  last_cpu_time = energest_type_time(ENERGEST_TYPE_CPU);
//  last_lpm_time = energest_type_time(ENERGEST_TYPE_LPM);
//  last_deep_lpm_time = energest_type_time(ENERGEST_TYPE_DEEP_LPM);
//  last_radio_tx_time = energest_type_time(ENERGEST_TYPE_TRANSMIT);
//  last_radio_rx_time = energest_type_time(ENERGEST_TYPE_LISTEN);
//  return energest_values_uint32;
//}
//
