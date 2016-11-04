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


#ifndef _UTILS_H
#define _UTILS_H 1

#ifdef HAVE_FEATURE_DEBUG

#include <stdio.h>
#include <stdlib.h>

#include "state.h"

#define IF_DEBUGGING(x) do { x ; } while (0)

/**
 * if we haven't initialized through start_pes() then try to do
 * something constructive.  Obviously can't use shmemi_trace()
 * because nothing has been initialized.
 *
 */

#define DEBUG_NAME(Name) const char *debug_name = Name

#define INIT_CHECK(subrname)                                            \
    IF_DEBUGGING(                                                       \
                 const int s = GET_STATE (pe_status);                   \
                 if (s != PE_RUNNING) {                                 \
                     fprintf (stderr,                                   \
                              "OpenSHMEM: Called %s() but %s\n",        \
                              subrname,                                 \
                              shmemi_state_as_string (s)                \
                              );                                        \
                     exit (EXIT_FAILURE);                               \
                     /* NOT REACHED */                                  \
                 }                                                      \
                                                                        )

/*
 * make sure a target PE is within the assigned range
 *
 */

#include "trace.h"

#define PE_RANGE_CHECK(pe, argpos, subrname)                            \
    IF_DEBUGGING(                                                       \
                 const int bot_pe = 0;                                  \
                 const int top_pe = GET_STATE (numpes) - 1;             \
                 if (pe < bot_pe || pe > top_pe) {                      \
                     shmemi_trace (SHMEM_LOG_FATAL,                     \
                                   "PE %d in argument #%d of %s() not"  \
                                   " within allocated range %d .. %d",  \
                                   pe, argpos, subrname,                \
                                   bot_pe, top_pe                       \
                                   );                                   \
                     /* NOT REACHED */                                  \
                 }                                                      \
                                                                        )

/*
 * check for symmetry of required addresses
 *
 */

#include "symmtest.h"

#define SYMMETRY_CHECK(addr, argpos, subrname)                          \
    IF_DEBUGGING(                                                       \
                 if (shmemi_symmetric_addr_lookup ((void *) addr,       \
                                                   GET_STATE (mype)) == NULL) \
                     {                                                  \
                         shmemi_trace (SHMEM_LOG_FATAL,                 \
                                       "%s(), argument #%d @ %p"        \
                                       " is not symmetric",             \
                                       subrname, argpos, addr           \
                                       );                               \
                         /* NOT REACHED */                              \
                     }                                                  \
                                                                        )
/*
 * sanity-check the length of put/get operations
 *
 */

#define TXRX_LENGTH_CHECK(len, argpos, subrname)                        \
    IF_DEBUGGING(                                                       \
                 if ((len) == 0) {                                      \
                     shmemi_trace (SHMEM_LOG_INFO,                      \
                                   "%s(), length in argument #%d"       \
                                   " is zero, call has no effect",      \
                                   subrname, argpos                     \
                                   );                                   \
                 }                                                      \
                 if ((len) < 0) {                                       \
                     shmemi_trace (SHMEM_LOG_INFO,                      \
                                   "%s(), length in argument #%d"       \
                                   " is negative, call has no effect",  \
                                   subrname, argpos                     \
                                   );                                   \
                 }                                                      \
                                                                        \
                                                                        )

#else /* ! HAVE_FEATURE_DEBUG */

#define DEBUG_NAME(name)
#define INIT_CHECK(subrname)
#define PE_RANGE_CHECK(pe, argpos, subrname)
#define SYMMETRY_CHECK(addr, argpos, subrname)
#define TXRX_LENGTH_CHECK(len, argpos, subrname)

#endif /* HAVE_FEATURE_DEBUG */

/*
 * how many elements in array T?
 *
 */
#define TABLE_SIZE(T) ( sizeof(T) / sizeof((T)[0]) )

/*
 * for branch prediction optimization
 *
 */
#define EXPR_LIKELY(expression) __builtin_expect(!!(expression), 1)
#define EXPR_UNLIKELY(expression) __builtin_expect(!!(expression), 0)

#endif /* _UTILS_H */
