#ifndef ENERGEST_HANDLER_H
#define ENERGEST_HANDLER_H

#include "sys/energest.h"
#include "sys/rtimer.h"
#include <stdint.h>
#include <stdbool.h>

uint32_t* energy_stats(bool print_output, unsigned int period_count);

void energest_handler_start(energest_type_t type);
void energest_handler_stop(energest_type_t type);


#endif /* ENERGEST_HANDLER_H */

//#ifndef ENERGEST_HANDLER_H
//#define ENERGEST_HANDLER_H
//
//#include "sys/energest.h"
//#include "sys/rtimer.h"
//#include <stdint.h>
//#include <stdbool.h>
//
////
////typedef struct {
////  uint32_t cpu_time;
////  uint32_t lpm_time;
////  uint32_t deep_lpm_time;
////  uint32_t radio_tx_time;
////  uint32_t radio_rx_time;
////  rtimer_clock_t period_time_ticks;
////  uint32_t period_time_seconds;
////  unsigned int period_count;
////} energest_values_t;
//
//uint32_t * energy_stats(bool print_output, unsigned int period_count);
////energest_values_t *energest_buffer = energy_stats(true, period_count);
//
//
//#endif /* ENERGEST_HANDLER_H */
