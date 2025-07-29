#ifndef __KCSTD_TIME_H__
#define __KCSTD_TIME_H__

#include "kcstd/types.h"

/** Identifier for system-wide realtime clock. */
#define CLOCK_REALTIME 0

/** Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC 1

/** High-resolution timer from the CPU. */
#define CLOCK_PROCESS_CPUTIME_ID 2

/** Thread-specific CPU-time clock. */
#define CLOCK_THREAD_CPUTIME_ID 3

/** Monotonic system-wide clock, not adjusted for frequency scaling. */
#define CLOCK_MONOTONIC_RAW 4

/** Identifier for system-wide realtime clock, updated only on ticks.  */
#define CLOCK_REALTIME_COARSE 5

/** Monotonic system-wide clock, updated only on ticks.  */
#define CLOCK_MONOTONIC_COARSE 6

/** Monotonic system-wide clock that includes time spent in suspension.  */
#define CLOCK_BOOTTIME 7

/** Like CLOCK_REALTIME but also wakes suspended system.  */
#define CLOCK_REALTIME_ALARM 8

/** Like CLOCK_BOOTTIME but also wakes suspended system.  */
#define CLOCK_BOOTTIME_ALARM 9

/** Like CLOCK_REALTIME but in International Atomic Time.  */
#define CLOCK_TAI 11

/** Max Rand Value */
#define RAND_MAX_31 ((size_t)1 << 31)

typedef struct {
  int64_t sec;
  int64_t nano_sec;
} time_spec;

typedef char clockid_t;

/** Keep it here instead syscalls.h
 *  because of the types clockid_t and time_spec
 */
extern int __ASM_CLOCK_GETTIME__(clockid_t clock_id, time_spec* ts);

extern size_t __TIME__SEED__;

void start_rand(size_t seed);

size_t rand();

int clock_get_time(clockid_t clock_id, time_spec* ts);

int64_t time(int64_t* addr);

#endif