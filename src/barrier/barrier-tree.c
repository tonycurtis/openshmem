/*
 *
 * Copyright (c) 2016
 *   Stony Brook University
 * Copyright (c) 2015 - 2016
 *   Los Alamos National Security, LLC.
 * Copyright (c) 2011 - 2016
 *   University of Houston System and UT-Battelle, LLC.
 * Copyright (c) 2009 - 2016
 *   Silicon Graphics International Corp.  SHMEM is copyrighted
 *   by Silicon Graphics International Corp. (SGI) The OpenSHMEM API
 *   (shmem) is released by Open Source Software Solutions, Inc., under an
 *   agreement with Silicon Graphics International Corp. (SGI).
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimers.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * o Neither the name of the University of Houston System,
 *   UT-Battelle, LLC. nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * o Neither the name of Los Alamos National Security, LLC, Los Alamos
 *   National Laboratory, LANL, the U.S. Government, nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "state.h"
#include "trace.h"

#include "shmem.h"

/*
 * Tree based barrier generates a binary tree with the PEs in the
 * active set.
 *
 */

static void
set_2tree (int PE_start, int PE_stride, int PE_size,
           int *parent, int *child_l, int *child_r, int my_pe)
{
    int max_pe = PE_start + PE_stride * (PE_size - 1);

    *child_l = 2 * (my_pe - PE_start) + PE_stride + PE_start;
    *child_r = *child_l + PE_stride;

    /* set parent */
    if (my_pe == PE_start) {
        *parent = -1;
    }
    else {
        *parent = (my_pe - PE_start - PE_stride) / 2;
        *parent -= *parent % PE_stride;
        *parent += PE_start;
    }

    if (*child_l > max_pe) {
        *child_l = *child_r = -1;
    }
    else if (*child_r > max_pe) {
        *child_r = -1;
    }

    shmemi_trace (SHMEM_LOG_BARRIER,
                  "set2tree: parent = %d, L_child = %d, R_child = %d",
                  *parent, *child_l, *child_r);
}

void
shmemi_barrier_tree (int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    int child_l, child_r, parent;
    const int step = 1 << logPE_stride;
    int my_pe = GET_STATE (mype);
    int no_children;
    long is_ready, lchild_ready, rchild_ready;


    is_ready = 1;
    lchild_ready = -1;
    rchild_ready = -1;

    shmem_long_wait_until (&pSync[0], SHMEM_CMP_EQ, SHMEM_SYNC_VALUE);
    shmem_long_wait_until (&pSync[1], SHMEM_CMP_EQ, SHMEM_SYNC_VALUE);

    /* printf("Tree barrier\n"); */
    set_2tree (PE_start, step, PE_size, &parent, &child_l, &child_r, my_pe);

    no_children = 0;

    shmemi_trace (SHMEM_LOG_BARRIER,
                  "before barrier, R_child = %d L_child = %d",
                  child_r, child_l);

    /* The actual barrier */

    if (PE_size > 1) {
        pSync[0] = 0;
        pSync[1] = 0;

        if (my_pe == PE_start) {
            pSync[0] = SHMEM_SYNC_VALUE;

            if (child_l != -1) {
                shmem_long_get (&lchild_ready, (const long *) &pSync[0],
                                1, child_l);
                while (lchild_ready != 0)
                    shmem_long_get (&lchild_ready, &pSync[0], 1, child_l);
                shmem_long_put (&pSync[0], &is_ready, 1, child_l);
                no_children = 1;
            }

            if (child_r != -1) {
                shmem_long_get (&rchild_ready, &pSync[0], 1, child_r);
                while (rchild_ready != 0)
                    shmem_long_get (&rchild_ready, &pSync[0], 1, child_r);
                shmem_long_put (&pSync[0], &is_ready, 1, child_r);
                no_children = 2;
            }

            shmem_long_wait_until (&pSync[1], SHMEM_CMP_EQ,
                                   (long) no_children);
            pSync[1] = SHMEM_SYNC_VALUE;

        }
        else {
            shmem_long_wait_until (&pSync[0], SHMEM_CMP_EQ, is_ready);

            shmemi_trace (SHMEM_LOG_BARRIER, "inside else");

            if (child_l != -1) {
                shmem_long_get (&lchild_ready, &pSync[0], 1, child_l);
                while (lchild_ready != 0)
                    shmem_long_get (&lchild_ready, &pSync[0], 1, child_l);
                shmem_long_put (&pSync[0], &is_ready, 1, child_l);
                no_children = 1;
            }

            if (child_r != -1) {
                shmem_long_get (&rchild_ready, &pSync[0], 1, child_r);
                while (rchild_ready != 0)
                    shmem_long_get (&rchild_ready, &pSync[0], 1, child_r);
                shmem_long_put (&pSync[0], &is_ready, 1, child_r);
                no_children = 2;
            }
            pSync[0] = SHMEM_SYNC_VALUE;

            if (no_children == 0) {
                pSync[1] = SHMEM_SYNC_VALUE;
                shmem_long_inc (&pSync[1], parent);
            }
            else {
                shmem_long_wait_until (&pSync[1], SHMEM_CMP_EQ,
                                       (long) no_children);
                pSync[1] = SHMEM_SYNC_VALUE;
                shmem_long_inc (&pSync[1], parent);
            }

        }
        shmemi_trace (SHMEM_LOG_BARRIER, "at the end of barrier");

    }
}
