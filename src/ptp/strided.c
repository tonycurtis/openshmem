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


#include "trace.h"
#include "utils.h"

#include "shmem.h"

#ifdef HAVE_FEATURE_PSHMEM
#include "pshmem.h"
#endif /* HAVE_FEATURE_PSHMEM */


/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern void shmem_complexf_p (complex float *addr, complex float value, int pe);
extern complex float shmem_complexf_g (complex float *addr, int pe);

extern void shmem_char_iput (char *target, const char *source, ptrdiff_t tst,
                             ptrdiff_t sst, size_t nelems, int pe);
extern void shmem_char_iget (char *target, const char *source, ptrdiff_t tst,
                             ptrdiff_t sst, size_t nelems, int pe);


#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_short_iput = pshmem_short_iput
#define shmem_short_iput pshmem_short_iput
#pragma weak shmem_char_iput = pshmem_char_iput
#define shmem_char_iput pshmem_char_iput
#pragma weak shmem_int_iput = pshmem_int_iput
#define shmem_int_iput pshmem_int_iput
#pragma weak shmem_long_iput = pshmem_long_iput
#define shmem_long_iput pshmem_long_iput
#pragma weak shmem_double_iput = pshmem_double_iput
#define shmem_double_iput pshmem_double_iput
#pragma weak shmem_float_iput = pshmem_float_iput
#define shmem_float_iput pshmem_float_iput
#pragma weak shmem_longdouble_iput = pshmem_longdouble_iput
#define shmem_longdouble_iput pshmem_longdouble_iput
#pragma weak shmem_longlong_iput = pshmem_longlong_iput
#define shmem_longlong_iput pshmem_longlong_iput
#pragma weak shmem_iput32 = pshmem_iput32
#define shmem_iput32 pshmem_iput32
#pragma weak shmem_iput64 = pshmem_iput64
#define shmem_iput64 pshmem_iput64
#pragma weak shmem_iput128 = pshmem_iput128
#define shmem_iput128 pshmem_iput128
#endif /* HAVE_FEATURE_PSHMEM */

/**
 *
 * void shmem_int_iput(symmetric int *target, const int *source,
 *                     ptrdiff_t target_stride,
 *                     ptrdiff_t source_stride,
 *                     size_t n_elems, int pe);
 *
 */

#define SHMEM_EMIT_IPUT(Name, Type)                                     \
    void                                                                \
    shmem_##Name##_iput (Type *target, const Type *source,              \
                         ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        DEBUG_NAME ("shmem_" #Name "_iput");                            \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        INIT_CHECK (debug_name);                                        \
        PE_RANGE_CHECK (pe, 6, debug_name);                             \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_##Name##_p (& (target[ti]), source[si], pe);          \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_EMIT_IPUT (char, char);
SHMEM_EMIT_IPUT (short, short);
SHMEM_EMIT_IPUT (int, int);
SHMEM_EMIT_IPUT (long, long);
SHMEM_EMIT_IPUT (double, double);
SHMEM_EMIT_IPUT (float, float);
SHMEM_EMIT_IPUT (longdouble, long double);
SHMEM_EMIT_IPUT (longlong, long long);
SHMEM_EMIT_IPUT (complexf, COMPLEXIFY (float));

void
shmem_iput32 (void *target, const void *source,
              ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe)
{
    shmem_int_iput (target, source, tst, sst, nelems, pe);
}

void
shmem_iput64 (void *target, const void *source,
              ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe)
{
    shmem_long_iput (target, source, tst, sst, nelems, pe);
}

void
shmem_iput128 (void *target, const void *source,
               ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe)
{
    shmem_longdouble_iput (target, source, tst, sst, nelems, pe);
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_char_iget = pshmem_char_iget
#define shmem_char_iget pshmem_char_iget
#pragma weak shmem_short_iget = pshmem_short_iget
#define shmem_short_iget pshmem_short_iget
#pragma weak shmem_int_iget = pshmem_int_iget
#define shmem_int_iget pshmem_int_iget
#pragma weak shmem_long_iget = pshmem_long_iget
#define shmem_long_iget pshmem_long_iget
#pragma weak shmem_double_iget = pshmem_double_iget
#define shmem_double_iget pshmem_double_iget
#pragma weak shmem_float_iget = pshmem_float_iget
#define shmem_float_iget pshmem_float_iget
#pragma weak shmem_longdouble_iget = pshmem_longdouble_iget
#define shmem_longdouble_iget pshmem_longdouble_iget
#pragma weak shmem_longlong_iget = pshmem_longlong_iget
#define shmem_longlong_iget pshmem_longlong_iget
#pragma weak shmem_iget32 = pshmem_iget32
#define shmem_iget32 pshmem_iget32
#pragma weak shmem_iget64 = pshmem_iget64
#define shmem_iget64 pshmem_iget64
#pragma weak shmem_iget128 = pshmem_iget128
#define shmem_iget128 pshmem_iget128
#endif /* HAVE_FEATURE_PSHMEM */

/**
 *
 * void shmem_int_iget(int *target, symmetric const int *source,
 *                     ptrdiff_t target_stride,
 *                     ptrdiff_t source_stride,
 *                     size_t n_elems, int pe);
 *
 */

#define SHMEM_EMIT_IGET(Name, Type)                                     \
    void                                                                \
    shmem_##Name##_iget (Type *target, const Type *source,              \
                         ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        DEBUG_NAME ("shmem_" #Name "_iget");                            \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        INIT_CHECK (debug_name);                                        \
        PE_RANGE_CHECK (pe, 6, debug_name);                             \
        for (i = 0; i < nelems; i += 1) {                               \
            target[ti] = shmem_##Name##_g ((Type *) & (source[si]), pe); \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_EMIT_IGET (char, char);
SHMEM_EMIT_IGET (short, short);
SHMEM_EMIT_IGET (int, int);
SHMEM_EMIT_IGET (long, long);
SHMEM_EMIT_IGET (double, double);
SHMEM_EMIT_IGET (float, float);
SHMEM_EMIT_IGET (longdouble, long double);
SHMEM_EMIT_IGET (longlong, long long);
SHMEM_EMIT_IGET (complexf, COMPLEXIFY (float));

void
shmem_iget32 (void *target, const void *source,
              ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe)
{
    shmem_int_iget (target, source, tst, sst, nelems, pe);
}

void
shmem_iget64 (void *target, const void *source,
              ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe)
{
    shmem_long_iget (target, source, tst, sst, nelems, pe);
}

void
shmem_iget128 (void *target, const void *source,
               ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe)
{
    shmem_longdouble_iget (target, source, tst, sst, nelems, pe);
}
