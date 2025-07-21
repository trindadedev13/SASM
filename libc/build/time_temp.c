#include "kcstd/time.h"

#include "kcstd/syscalls.h"

size_t __TIME__SEED__ = 0;

void start_rand(size_t seed) {
  __TIME__SEED__ = seed;
}

size_t rand() {
  __TIME__SEED__ = (1103515245 * __TIME__SEED__ + 12345) % RAND_MAX_31;
  return __TIME__SEED__;
}

int clock_get_time(clockid_t clock_id, time_spec* ts) {
  return __ASM_CLOCK_GETTIME__(clock_id, ts);
}

int64_t time(int64_t* addr) {
  time_spec ts;
  if (clock_get_time(CLOCK_REALTIME, &ts) != 0)
    return -1;
  if (addr != NULL)
    *addr = ts.sec;
  return ts.sec;
}