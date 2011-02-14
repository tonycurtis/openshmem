#include "state.h"
#include "comms.h"
#include "trace.h"
#include "comms.h"
#include "hooks.h"

#include "pshmem.h"

void
pshmem_barrier_all_naive(void)
{
  __comms_barrier_all();
}

void
pshmem_barrier_naive(int PE_start, int logPE_stride, int PE_size, long *pSync)
{
  __comms_fence();

  if (__state.mype == PE_start) {
    const int step = 1 << logPE_stride;
    int i;
    int thatpe;
    /* root signals everyone else */
    for (thatpe = PE_start, i = 1; i < PE_size; i += 1) {
      thatpe += step;
      pshmem_long_p(& pSync[thatpe], ~ _SHMEM_SYNC_VALUE, thatpe);
    }
    /* root waits for ack from everyone else */
    for (thatpe = PE_start, i = 1; i < PE_size; i += 1) {
      thatpe += step;
      pshmem_wait(& pSync[thatpe], ~ _SHMEM_SYNC_VALUE);
    }
  }
  else {
    /* non-root waits for root to signal, then tell root we're ready */
    pshmem_wait(& pSync[__state.mype], _SHMEM_SYNC_VALUE);
    pshmem_long_p(& pSync[__state.mype], _SHMEM_SYNC_VALUE, PE_start);
  }
  /* restore pSync values */
  pSync[__state.mype] = _SHMEM_SYNC_VALUE;

  __comms_fence();
}
