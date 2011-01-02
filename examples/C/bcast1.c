#include <stdio.h>
#include <stdlib.h>

#include <mpp/shmem.h>

int
main(void)
{
  int i;
  long *target;
  long *source;
  long *pSync;
  int me, npes;

  source = (long *) malloc( npes * sizeof(*source) );
  for (i = 0; i < npes; i += 1) {
    source[i] = i + 1;
  }

  start_pes(0);
  me = _my_pe();
  npes = _num_pes();

  target = (long *) shmalloc( npes * sizeof(*target) );
  for (i = 0; i < npes; i += 1) {
    target[i] = -999;
  }
  shmem_barrier_all();

  shmem_sync_init(pSync);

  shmem_broadcast64(target, source, npes, 0, 0, 0, npes, pSync);

  // shmem_barrier_all();

  for (i = 0; i < npes; i++) {
    printf("%-8d %ld\n", me, target[i]);
  }

  shmem_barrier_all();
  
  shfree(target);
  shfree(pSync);
  free(source);

  return 0;
}
