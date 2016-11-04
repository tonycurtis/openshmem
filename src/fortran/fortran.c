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


/*
 * This is the Fortran interface.  Parameter-less routines are
 * the same as C, but with the standard underscore mangling we all love.
 *
 * Also provides pass-by-reference wrappers to translate C value params
 *
 */

#include <stdint.h>
#include <stddef.h>             /* ptrdiff_t */
#include <string.h>

#include "fortran-common.h"

#include "shmem.h"

#ifdef HAVE_FEATURE_PSHMEM
#include "pshmem.h"
#endif /* HAVE_FEATURE_PSHMEM */

#ifdef HAVE_FEATURE_EXPERIMENTAL
#include "shmemx.h"
#include "pshmemx.h"
#endif /* HAVE_FEATURE_EXPERIMENTAL */

/**
 *
 * init & query functions
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak start_pes_ = pstart_pes_
#define start_pes_ pstart_pes_

#pragma weak my_pe_ = pmy_pe_
#define my_pe_ pmy_pe_

#pragma weak num_pes_ = pnum_pes_
#define num_pes_ pnum_pes_

#pragma weak shmem_my_pe_ = pshmem_my_pe_
#define shmem_my_pe_ pshmem_my_pe_

#pragma weak shmem_n_pes_ = pshmem_n_pes_
#define shmem_n_pes_ pshmem_n_pes_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (start_pes) (int *npes /* unused */ )
{
    shmem_init ();
}

FORTRANIFY_VOID_VOID (shmem_init);
FORTRANIFY_VOID_VOID (shmem_finalize);

void
FORTRANIFY (shmem_global_exit) (int *status)
{
    shmem_global_exit (*status);
}

#define SHMEM_FORTRAN_QUERY_PE(FName, CName)    \
    int                                         \
    FORTRANIFY(FName)(void)                     \
    {                                           \
        return CName ();                        \
    }

SHMEM_FORTRAN_QUERY_PE (my_pe, shmem_my_pe);
SHMEM_FORTRAN_QUERY_PE (num_pes, shmem_n_pes);
#if 0
/* invalid Fortran symbol names */
SHMEM_FORTRAN_QUERY_PE (_my_pe, shmem_my_pe);
SHMEM_FORTRAN_QUERY_PE (_num_pes, shmem_n_pes);
#endif

SHMEM_FORTRAN_QUERY_PE (shmem_my_pe, shmem_my_pe);
SHMEM_FORTRAN_QUERY_PE (shmem_n_pes, shmem_n_pes);



/*
 * puts and gets
 */

/*
 * Routines that are needed for Fortran, but aren't currently in the C
 * API
 */

extern void shmem_complexf_put (COMPLEXIFY (float) * target,
                                const COMPLEXIFY (float) * source,
                                size_t nelems, int pe);
extern void shmem_complexf_get (COMPLEXIFY (float) * target,
                                const COMPLEXIFY (float) * source,
                                size_t nelems, int pe);
extern void shmem_complexf_iput (COMPLEXIFY (float) * target,
                                 const COMPLEXIFY (float) * source,
                                 ptrdiff_t tst, ptrdiff_t sst, size_t nelems,
                                 int pe);
extern void shmem_complexf_iget (COMPLEXIFY (float) * target,
                                 const COMPLEXIFY (float) * source,
                                 ptrdiff_t tst, ptrdiff_t sst, size_t nelems,
                                 int pe);


#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_character_put_ = pshmem_character_put_
#define shmem_character_put_ pshmem_character_put_
#pragma weak shmem_double_put_ = pshmem_double_put_
#define shmem_double_put_ pshmem_double_put_
#pragma weak shmem_integer_put_ = pshmem_integer_put_
#define shmem_integer_put_ pshmem_integer_put_
#pragma weak shmem_logical_put_ = pshmem_logical_put_
#define shmem_logical_put_ pshmem_logical_put_
#pragma weak shmem_real_put_ = pshmem_real_put_
#define shmem_real_put_ pshmem_real_put_
#pragma weak shmem_complex_put_ = pshmem_complex_put_
#define shmem_complex_put_ pshmem_complex_put_
#pragma weak shmem_put4_ = pshmem_put4_
#define shmem_put4_ pshmem_put4_
#pragma weak shmem_put8_ = pshmem_put8_
#define shmem_put8_ pshmem_put8_
#pragma weak shmem_put32_ = pshmem_put32_
#define shmem_put32_ pshmem_put32_
#pragma weak shmem_put64_ = pshmem_put64_
#define shmem_put64_ pshmem_put64_
#pragma weak shmem_put128_ = pshmem_put128_
#define shmem_put128_ pshmem_put128_
#pragma weak shmem_putmem_ = pshmem_putmem_
#define shmem_putmem_ pshmem_putmem_

#define shmem_put_ pshmem_put_
#pragma weak shmem_put_ = pshmem_put_
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_FORTRAN_PUT(FName, CName, CType)                          \
    void                                                                \
    FORTRANIFY(shmem_##FName##_put) (CType *target, const CType *source, \
                                     int *size, int *pe)                \
    {                                                                   \
        shmem_##CName##_put (target, source, *size, *pe);               \
    }

#define SHMEM_FORTRAN_PUT_SIZE(Size, CName, CType)                      \
    void                                                                \
    FORTRANIFY(shmem_put##Size) (CType *target, const CType *source,    \
                                 int *size, int *pe)                    \
    {                                                                   \
        shmem_##CName##_put (target, source, *size, *pe);               \
    }

SHMEM_FORTRAN_PUT (character, char, char);
SHMEM_FORTRAN_PUT (double, double, double);
SHMEM_FORTRAN_PUT (integer, int, int);
SHMEM_FORTRAN_PUT (logical, int, int);
SHMEM_FORTRAN_PUT (real, float, float);
SHMEM_FORTRAN_PUT (complex, complexf, COMPLEXIFY (float));
SHMEM_FORTRAN_PUT_SIZE (4, int, int);
SHMEM_FORTRAN_PUT_SIZE (8, long, long);
SHMEM_FORTRAN_PUT_SIZE (32, int, int);
SHMEM_FORTRAN_PUT_SIZE (64, long, long);
SHMEM_FORTRAN_PUT_SIZE (128, longdouble, long double);

void
FORTRANIFY (shmem_putmem) (void *target, const void *source, int *size, int *pe)
{
    shmem_putmem (target, source, *size, *pe);
}

void
FORTRANIFY (shmem_put) (long *target, const long *source, int *size, int *pe)
{
    shmem_long_put (target, source, *size, *pe);
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_character_get_ = pshmem_character_get_
#define shmem_character_get_ pshmem_character_get_
#pragma weak shmem_double_get_ = pshmem_double_get_
#define shmem_double_get_ pshmem_double_get_
#pragma weak shmem_integer_get_ = pshmem_integer_get_
#define shmem_integer_get_ pshmem_integer_get_
#pragma weak shmem_logical_get_ = pshmem_logical_get_
#define shmem_logical_get_ pshmem_logical_get_
#pragma weak shmem_real_get_ = pshmem_real_get_
#define shmem_real_get_ pshmem_real_get_
#pragma weak shmem_complex_get_ = pshmem_complex_get_
#define shmem_complex_get_ pshmem_complex_get_
#pragma weak shmem_get4_ = pshmem_get4_
#define shmem_get4_ pshmem_get4_
#pragma weak shmem_get8_ = pshmem_get8_
#define shmem_get8_ pshmem_get8_
#pragma weak shmem_get32_ = pshmem_get32_
#define shmem_get32_ pshmem_get32_
#pragma weak shmem_get64_ = pshmem_get64_
#define shmem_get64_ pshmem_get64_
#pragma weak shmem_get128_ = pshmem_get128_
#define shmem_get128_ pshmem_get128_
#pragma weak shmem_getmem_ = pshmem_getmem_
#define shmem_getmem_ pshmem_getmem_

#pragma weak shmem_get_ = pshmem_get_
#define shmem_get_ pshmem_get_
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_FORTRAN_GET(FName, CName, CType)                          \
    void                                                                \
    FORTRANIFY(shmem_##FName##_get) (CType *target, const CType *source, \
                                     int *size, int *pe)                \
    {                                                                   \
        shmem_##CName##_get (target, source, *size, *pe);               \
    }

#define SHMEM_FORTRAN_GET_SIZE(Size, CName, CType)                      \
    void                                                                \
    FORTRANIFY(shmem_get##Size) (CType *target, const CType *source,    \
                                 int *size, int *pe)                    \
    {                                                                   \
        shmem_##CName##_get (target, source, *size, *pe);               \
    }

SHMEM_FORTRAN_GET (character, char, char);
SHMEM_FORTRAN_GET (double, double, double);
SHMEM_FORTRAN_GET (integer, int, int);
SHMEM_FORTRAN_GET (logical, int, int);
SHMEM_FORTRAN_GET (real, float, float);
SHMEM_FORTRAN_GET (complex, complexf, COMPLEXIFY (float));
SHMEM_FORTRAN_GET_SIZE (4, int, int);
SHMEM_FORTRAN_GET_SIZE (8, long, long);
SHMEM_FORTRAN_GET_SIZE (32, int, int);
SHMEM_FORTRAN_GET_SIZE (64, long, long);
SHMEM_FORTRAN_GET_SIZE (128, longdouble, long double);

void
FORTRANIFY (shmem_getmem) (void *target, const void *source, int *size, int *pe)
{
    shmem_getmem (target, source, *size, *pe);
}

void
FORTRANIFY (shmem_get) (long *target, const long *source, int *size, int *pe)
{
    shmem_long_get (target, source, *size, *pe);
}

/*
 * non-blocking implicit put/get
 */

/*
 * Routines that are needed for Fortran, but aren't currently in the C
 * API
 */

extern void shmem_complexf_put_nbi (COMPLEXIFY (float) * target,
                                    const COMPLEXIFY (float) * source,
                                    size_t nelems, int pe);
extern void shmem_complexf_get_nbi (COMPLEXIFY (float) * target,
                                    const COMPLEXIFY (float) * source,
                                    size_t nelems, int pe);
extern void shmem_complexf_iput_nbi (COMPLEXIFY (float) * target,
                                     const COMPLEXIFY (float) * source,
                                     ptrdiff_t tst, ptrdiff_t sst,
                                     size_t nelems, int pe);
extern void shmem_complexf_iget_nbi (COMPLEXIFY (float) * target,
                                     const COMPLEXIFY (float) * source,
                                     ptrdiff_t tst, ptrdiff_t sst,
                                     size_t nelems, int pe);

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_character_put_nbi_ = pshmem_character_put_nbi_
#define shmem_character_put_nbi_ pshmem_character_put_nbi_
#pragma weak shmem_double_put_nbi_ = pshmem_double_put_nbi_
#define shmem_double_put_nbi_ pshmem_double_put_nbi_
#pragma weak shmem_integer_put_nbi_ = pshmem_integer_put_nbi_
#define shmem_integer_put_nbi_ pshmem_integer_put_nbi_
#pragma weak shmem_logical_put_nbi_ = pshmem_logical_put_nbi_
#define shmem_logical_put_nbi_ pshmem_logical_put_nbi_
#pragma weak shmem_real_put_nbi_ = pshmem_real_put_nbi_
#define shmem_real_put_nbi_ pshmem_real_put_nbi_
#pragma weak shmem_complex_put_nbi_ = pshmem_complex_put_nbi_
#define shmem_complex_put_nbi_ pshmem_complex_put_nbi_
#pragma weak shmem_put4_nbi_ = pshmem_put4_nbi_
#define shmem_put4_nbi_ pshmem_put4_nbi_
#pragma weak shmem_put8_nbi_ = pshmem_put8_nbi_
#define shmem_put8_nbi_ pshmem_put8_nbi_
#pragma weak shmem_put32_nbi_ = pshmem_put32_nbi_
#define shmem_put32_nbi_ pshmem_put32_nbi_
#pragma weak shmem_put64_nbi_ = pshmem_put64_nbi_
#define shmem_put64_nbi_ pshmem_put64_nbi_
#pragma weak shmem_put128_nbi_ = pshmem_put128_nbi_
#define shmem_put128_nbi_ pshmem_put128_nbi_
#pragma weak shmem_putmem_nbi_ = pshmem_putmem_nbi_
#define shmem_putmem_nbi_ pshmem_putmem_nbi_
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_FORTRAN_PUT_NBI(FName, CName, CType)                      \
    void                                                                \
    FORTRANIFY(shmem_##FName##_put_nbi) (CType *target,                 \
                                         const CType *source,           \
                                         int *size, int *pe)            \
    {                                                                   \
        shmem_##CName##_put_nbi (target, source, *size, *pe);           \
    }

#define SHMEM_FORTRAN_PUT_SIZE_NBI(Size, CName, CType)                  \
    void                                                                \
    FORTRANIFY(shmem_put##Size##_nbi) (CType *target,                   \
                                       const CType *source,             \
                                       int *size, int *pe)              \
    {                                                                   \
        shmem_##CName##_put_nbi (target, source, *size, *pe);           \
    }

SHMEM_FORTRAN_PUT_NBI (character, char, char);
SHMEM_FORTRAN_PUT_NBI (double, double, double);
SHMEM_FORTRAN_PUT_NBI (integer, int, int);
SHMEM_FORTRAN_PUT_NBI (logical, int, int);
SHMEM_FORTRAN_PUT_NBI (real, int, int);
SHMEM_FORTRAN_PUT_NBI (complex, complexf, COMPLEXIFY (float));
SHMEM_FORTRAN_PUT_SIZE_NBI (4, int, int);
SHMEM_FORTRAN_PUT_SIZE_NBI (8, long, long);
SHMEM_FORTRAN_PUT_SIZE_NBI (32, int, int);
SHMEM_FORTRAN_PUT_SIZE_NBI (64, long, long);
SHMEM_FORTRAN_PUT_SIZE_NBI (128, longlong, long long);


void
FORTRANIFY (shmem_putmem_nbi) (void *target, const void *source,
                               int *size, int *pe)
{
    shmem_putmem_nbi (target, source, *size, *pe);
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_character_get_nbi_ = pshmem_character_get_nbi_
#define shmem_character_get_nbi_ pshmem_character_get_nbi_
#pragma weak shmem_double_get_nbi_ = pshmem_double_get_nbi_
#define shmem_double_get_nbi_ pshmem_double_get_nbi_
#pragma weak shmem_integer_get_nbi_ = pshmem_integer_get_nbi_
#define shmem_integer_get_nbi_ pshmem_integer_get_nbi_
#pragma weak shmem_logical_get_nbi_ = pshmem_logical_get_nbi_
#define shmem_logical_get_nbi_ pshmem_logical_get_nbi_
#pragma weak shmem_real_get_nbi_ = pshmem_real_get_nbi_
#define shmem_real_get_nbi_ pshmem_real_get_nbi_
#pragma weak shmem_complex_get_nbi_ = pshmem_complex_get_nbi_
#define shmem_complex_get_nbi_ pshmem_complex_get_nbi_
#pragma weak shmem_get4_nbi_ = pshmem_get4_nbi_
#define shmem_get4_nbi_ pshmem_get4_nbi_
#pragma weak shmem_get8_nbi_ = pshmem_get8_nbi_
#define shmem_get8_nbi_ pshmem_get8_nbi_
#pragma weak shmem_get32_nbi_ = pshmem_get32_nbi_
#define shmem_get32_nbi_ pshmem_get32_nbi_
#pragma weak shmem_get64_nbi_ = pshmem_get64_nbi_
#define shmem_get64_nbi_ pshmem_get64_nbi_
#pragma weak shmem_get128_nbi_ = pshmem_get128_nbi_
#define shmem_get128_nbi_ pshmem_get128_nbi_
#pragma weak shmem_getmem_nbi_ = pshmem_getmem_nbi_
#define shmem_getmem_nbi_ pshmem_getmem_nbi_
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_FORTRAN_GET_NBI(FName, CName, CType)                      \
    void                                                                \
    FORTRANIFY(shmem_##FName##_get_nbi) (CType *target,                 \
                                         const CType *source,           \
                                         int *size, int *pe)            \
    {                                                                   \
        shmem_##CName##_get_nbi (target, source, *size, *pe);           \
    }

#define SHMEM_FORTRAN_GET_SIZE_NBI(Size, CName, CType)                  \
    void                                                                \
    FORTRANIFY(shmem_get##Size##_nbi) (CType *target,                   \
                                       const CType *source,             \
                                       int *size, int *pe)              \
    {                                                                   \
        shmem_##CName##_get_nbi (target, source, *size, *pe);           \
    }

SHMEM_FORTRAN_GET_NBI (character, char, char);
SHMEM_FORTRAN_GET_NBI (double, double, double);
SHMEM_FORTRAN_GET_NBI (integer, int, int);
SHMEM_FORTRAN_GET_NBI (logical, int, int);
SHMEM_FORTRAN_GET_NBI (real, int, int);
SHMEM_FORTRAN_GET_NBI (complex, complexf, COMPLEXIFY (float));
SHMEM_FORTRAN_GET_SIZE_NBI (4, int, int);
SHMEM_FORTRAN_GET_SIZE_NBI (8, long, long);
SHMEM_FORTRAN_GET_SIZE_NBI (32, int, int);
SHMEM_FORTRAN_GET_SIZE_NBI (64, long, long);
SHMEM_FORTRAN_GET_SIZE_NBI (128, longlong, long long);

void
FORTRANIFY (shmem_getmem_nbi) (void *target, const void *source,
                               int *size, int *pe)
{
    shmem_getmem_nbi (target, source, *size, *pe);
}

/*
 * strided puts
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_character_iput_ = pshmem_character_iput_
#define shmem_character_iput_ pshmem_character_iput_
#pragma weak shmem_double_iput_ = pshmem_double_iput_
#define shmem_double_iput_ pshmem_double_iput_
#pragma weak shmem_integer_iput_ = pshmem_integer_iput_
#define shmem_integer_iput_ pshmem_integer_iput_
#pragma weak shmem_logical_iput_ = pshmem_logical_iput_
#define shmem_logical_iput_ pshmem_logical_iput_
#pragma weak shmem_real_iput_ = pshmem_real_iput_
#define shmem_real_iput_ pshmem_real_iput_
#pragma weak shmem_iput4_ = pshmem_iput4_
#define shmem_iput4_ pshmem_iput4_
#pragma weak shmem_iput8_ = pshmem_iput8_
#define shmem_iput8_ pshmem_iput8_
#pragma weak shmem_iput32_ = pshmem_iput32_
#define shmem_iput32_ pshmem_iput32_
#pragma weak shmem_iput64_ = pshmem_iput64_
#define shmem_iput64_ pshmem_iput64_
#pragma weak shmem_iput128_ = pshmem_iput128_
#define shmem_iput128_ pshmem_iput128_
#pragma weak shmem_complex_iput_ = pshmem_complex_iput_
#define shmem_complex_iput_ pshmem_complex_iput_
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_FORTRAN_IPUT(Name, CName, CType)                          \
    void                                                                \
    FORTRANIFY(shmem_##Name##_iput) (CType *target, const CType *source, \
                                     int *tst, int *sst,                \
                                     int *size, int *pe)                \
    {                                                                   \
        shmem_##CName##_iput (target, source, *tst, *sst, *size, *pe);  \
    }

#include <stdio.h>

#define SHMEM_FORTRAN_IPUT_SIZE(Size, CName, CType)                     \
    void                                                                \
    FORTRANIFY(shmem_iput##Size) (CType *target, const CType *source,   \
                                  int *tst, int *sst,                   \
                                  int *size, int *pe)                   \
    {                                                                   \
        shmem_##CName##_iput (target, source, *tst, *sst, *size, *pe);  \
    }

SHMEM_FORTRAN_IPUT (character, char, char);
SHMEM_FORTRAN_IPUT (double, double, double);
SHMEM_FORTRAN_IPUT (integer, int, int);
SHMEM_FORTRAN_IPUT (logical, int, int);
SHMEM_FORTRAN_IPUT (real, int, int);
SHMEM_FORTRAN_IPUT_SIZE (4, int, int);
SHMEM_FORTRAN_IPUT_SIZE (8, long, long);
SHMEM_FORTRAN_IPUT_SIZE (32, int, int);
SHMEM_FORTRAN_IPUT_SIZE (64, long, long);
SHMEM_FORTRAN_IPUT_SIZE (128, longdouble, long double);

SHMEM_FORTRAN_IPUT (complex, complexf, COMPLEXIFY (float));

/*
 * strided gets
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_character_iget_ = pshmem_character_iget_
#define shmem_character_iget_ pshmem_character_iget_
#pragma weak shmem_double_iget_ = pshmem_double_iget_
#define shmem_double_iget_ pshmem_double_iget_
#pragma weak shmem_integer_iget_ = pshmem_integer_iget_
#define shmem_integer_iget_ pshmem_integer_iget_
#pragma weak shmem_logical_iget_ = pshmem_logical_iget_
#define shmem_logical_iget_ pshmem_logical_iget_
#pragma weak shmem_real_iget_ = pshmem_real_iget_
#define shmem_real_iget_ pshmem_real_iget_
#pragma weak shmem_iget4_ = pshmem_iget4_
#define shmem_iget4_ pshmem_iget4_
#pragma weak shmem_iget8_ = pshmem_iget8_
#define shmem_iget8_ pshmem_iget8_
#pragma weak shmem_iget32_ = pshmem_iget32_
#define shmem_iget32_ pshmem_iget32_
#pragma weak shmem_iget64_ = pshmem_iget64_
#define shmem_iget64_ pshmem_iget64_
#pragma weak shmem_iget128_ = pshmem_iget128_
#define shmem_iget128_ pshmem_iget128_
#pragma weak shmem_complex_iget_ = pshmem_complex_iget_
#define shmem_complex_iget_ pshmem_complex_iget_
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEM_FORTRAN_IGET(Name, CName, CType)                          \
    void                                                                \
    FORTRANIFY(shmem_##Name##_iget) (CType *target, const CType *source, \
                                     int *tst, int *sst,                \
                                     int *size, int *pe)                \
    {                                                                   \
        shmem_##CName##_iget(target, source, *tst, *sst, *size, *pe);   \
    }

#define SHMEM_FORTRAN_IGET_SIZE(Size, CName, CType)                     \
    void                                                                \
    FORTRANIFY(shmem_iget##Size) (CType *target, const CType *source,   \
                                  int *tst, int *sst,                   \
                                  int *size, int *pe)                   \
    {                                                                   \
        shmem_##CName##_iget(target, source, *tst, *sst, *size, *pe);   \
    }

SHMEM_FORTRAN_IGET (character, char, char);
SHMEM_FORTRAN_IGET (double, double, double);
SHMEM_FORTRAN_IGET (integer, int, int);
SHMEM_FORTRAN_IGET (logical, int, int);
SHMEM_FORTRAN_IGET (real, int, int);
SHMEM_FORTRAN_IGET_SIZE (4, int, int);
SHMEM_FORTRAN_IGET_SIZE (8, long, long);
SHMEM_FORTRAN_IGET_SIZE (32, int, int);
SHMEM_FORTRAN_IGET_SIZE (64, long, long);
SHMEM_FORTRAN_IGET_SIZE (128, longdouble, long double);

SHMEM_FORTRAN_IGET (complex, complexf, COMPLEXIFY (float));

#if 0

#pragma weak shmem_nodename_ = pshmem_nodename_
#define shmem_nodename_ pshmem_nodename_

char *
FORTRANIFY (shmem_nodename) (void)
{
    return shmem_nodename ();
}

#endif /* 0: commented out */


/*
 * accessibility
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_addr_accessible_ = pshmem_addr_accessible_
#define shmem_addr_accessible_ pshmem_addr_accessible_
#pragma weak shmem_pe_accessible_ = pshmem_pe_accessible_
#define shmem_pe_accessible_ pshmem_pe_accessible_
#pragma weak shmem_ptr_ = pshmem_ptr_
#define shmem_ptr_ pshmem_ptr_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmem_addr_accessible) (void *addr, int *pe)
{
    return shmem_addr_accessible (addr, *pe);
}

int
FORTRANIFY (shmem_pe_accessible) (int *pe)
{
    return shmem_pe_accessible (*pe);
}

void *
FORTRANIFY (shmem_ptr) (void *target, int *pe)
{
    return shmem_ptr (target, *pe);
}


/*
 * barriers & fences
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_barrier_ = pshmem_barrier_
#define shmem_barrier_ pshmem_barrier_
#pragma weak shmem_barrier_all_ = pshmem_barrier_all_
#define shmem_barrier_all_ pshmem_barrier_all_
#pragma weak shmem_fence_ = pshmem_fence_
#define shmem_fence_ pshmem_fence_
#pragma weak shmem_quiet_ = pshmem_quiet_
#define shmem_quiet_ pshmem_quiet_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_barrier) (int *PE_start, int *logPE_stride, int *PE_size,
                            int *pSync)
{
    shmem_barrier (*PE_start, *logPE_stride, *PE_size, (long *) pSync);
}

FORTRANIFY_VOID_VOID (shmem_barrier_all);
FORTRANIFY_VOID_VOID (shmem_fence);
FORTRANIFY_VOID_VOID (shmem_quiet);

/*
 * wait operations
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_wait_until_ = pshmem_int4_wait_until_
#define shmem_int4_wait_until_ pshmem_int4_wait_until_
#pragma weak shmem_int8_wait_until_ = pshmem_int8_wait_until_
#define shmem_int8_wait_until_ pshmem_int8_wait_until_
#pragma weak shmem_wait_until_ = pshmem_wait_until_
#define shmem_wait_until_ pshmem_wait_until_

#pragma weak shmem_int4_wait_ = pshmem_int4_wait_
#define shmem_int4_wait_ pshmem_int4_wait_
#pragma weak shmem_int8_wait_ = pshmem_int8_wait_
#define shmem_int8_wait_ pshmem_int8_wait_
#pragma weak shmem_wait_ = pshmem_wait_
#define shmem_wait_ pshmem_wait_
#endif /* HAVE_FEATURE_PSHMEM */

#define FORTRANIFY_WAIT_UNTIL(Name, Type)                               \
    void                                                                \
    FORTRANIFY(shmem_##Name##_wait_until) (Type *ivar, int *cmp,        \
                                           Type *cmp_value)             \
    {                                                                   \
        shmem_##Type##_wait_until (ivar, *cmp, *cmp_value);             \
    }

#define FORTRANIFY_WAIT(Name, Type)                                 \
    void                                                            \
    FORTRANIFY(shmem_##Name##_wait) (Type *ivar, Type *cmp_value)   \
    {                                                               \
        shmem_##Type##_wait (ivar, *cmp_value);                     \
    }

FORTRANIFY_WAIT_UNTIL (int4, int);
FORTRANIFY_WAIT_UNTIL (int8, long);
FORTRANIFY_WAIT (int4, int);
FORTRANIFY_WAIT (int8, long);

void
FORTRANIFY (shmem_wait_until) (int *ivar, int *cmp, int *cmp_value)
{
    shmem_int_wait_until (ivar, *cmp, *cmp_value);
}

void
FORTRANIFY (shmem_wait) (int *ivar, int *cmp_value)
{
    shmem_int_wait (ivar, *cmp_value);
}

/*
 * atomics
 *
 */

/*
 * incs
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_inc_ = pshmem_int4_inc_
#define shmem_int4_inc_ pshmem_int4_inc_
#pragma weak shmem_int8_inc_ = pshmem_int8_inc_
#define shmem_int8_inc_ pshmem_int8_inc_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_int4_inc) (int *target, int *pe)
{
    shmem_int_inc (target, *pe);
}

void
FORTRANIFY (shmem_int8_inc) (long *target, int *pe)
{
    shmem_long_inc (target, *pe);
}

/*
 * fincs
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_finc_ = pshmem_int4_finc_
#define shmem_int4_finc_ pshmem_int4_finc_
#pragma weak shmem_int8_finc_ = pshmem_int8_finc_
#define shmem_int8_finc_ pshmem_int8_finc_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmem_int4_finc) (int *target, int *pe)
{
    return shmem_int_finc (target, *pe);
}

long
FORTRANIFY (shmem_int8_finc) (long *target, int *pe)
{
    return shmem_long_finc (target, *pe);
}

/*
 * adds
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_add_ = pshmem_int4_add_
#define shmem_int4_add_ pshmem_int4_add_
#pragma weak shmem_int8_add_ = pshmem_int8_add_
#define shmem_int8_add_ pshmem_int8_add_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_int4_add) (int *target, int *value, int *pe)
{
    shmem_int_add (target, *value, *pe);
}

void
FORTRANIFY (shmem_int8_add) (long *target, long *value, int *pe)
{
    shmem_long_add (target, *value, *pe);
}

/*
 * fadds
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_fadd_ = pshmem_int4_fadd_
#define shmem_int4_fadd_ pshmem_int4_fadd_
#pragma weak shmem_int8_fadd_ = pshmem_int8_fadd_
#define shmem_int8_fadd_ pshmem_int8_fadd_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmem_int4_fadd) (int *target, int *value, int *pe)
{
    return shmem_int_fadd (target, *value, *pe);
}

long
FORTRANIFY (shmem_int8_fadd) (long *target, long *value, int *pe)
{
    return shmem_long_fadd (target, *value, *pe);
}


/*
 * swaps
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_swap_ = pshmem_int4_swap_
#define shmem_int4_swap_ pshmem_int4_swap_
#pragma weak shmem_int8_swap_ = pshmem_int8_swap_
#define shmem_int8_swap_ pshmem_int8_swap_
#pragma weak shmem_real4_swap_ = pshmem_real4_swap_
#define shmem_real4_swap_ pshmem_real4_swap_
#pragma weak shmem_real8_swap_ = pshmem_real8_swap_
#define shmem_real8_swap_ pshmem_real8_swap_
#pragma weak shmem_swap_ = pshmem_swap_
#define shmem_swap_ pshmem_swap_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmem_int4_swap) (int *target, int *value, int *pe)
{
    return shmem_int_swap (target, *value, *pe);
}

long
FORTRANIFY (shmem_int8_swap) (long *target, long *value, int *pe)
{
    return shmem_long_swap (target, *value, *pe);
}

float
FORTRANIFY (shmem_real4_swap) (float *target, float *value, int *pe)
{
    return shmem_float_swap (target, *value, *pe);
}

double
FORTRANIFY (shmem_real8_swap) (double *target, double *value, int *pe)
{
    return shmem_double_swap (target, *value, *pe);
}

int
FORTRANIFY (shmem_swap) (int *target, int *value, int *pe)
{
    return shmem_int_swap (target, *value, *pe);
}

/*
 * cswaps
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_cswap_ = pshmem_int4_cswap_
#define shmem_int4_cswap_ pshmem_int4_cswap_
#pragma weak shmem_int8_cswap_ = pshmem_int8_cswap_
#define shmem_int8_cswap_ pshmem_int8_cswap_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmem_int4_cswap) (int *target, int *cond, int *value, int *pe)
{
    return shmem_int_cswap (target, *cond, *value, *pe);
}

long
FORTRANIFY (shmem_int8_cswap) (long *target, long *cond, long *value, int *pe)
{
    return shmem_long_cswap (target, *cond, *value, *pe);
}

/*
 * fetch/set
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_fetch_ = pshmem_int4_fetch_
#define shmem_int4_fetch_ pshmem_int4_fetch_
#pragma weak shmem_int8_fetch_ = pshmem_int8_fetch_
#define shmem_int8_fetch_ pshmem_int8_fetch_
#pragma weak shmem_real4_fetch_ = pshmem_real4_fetch_
#define shmem_real4_fetch_ pshmem_real4_fetch_
#pragma weak shmem_real8_fetch_ = pshmem_real8_fetch_
#define shmem_real8_fetch_ pshmem_real8_fetch_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmem_int4_fetch) (int *target, int *pe)
{
    return shmem_int_fetch (target, *pe);
}

long
FORTRANIFY (shmem_int8_fetch) (long *target, int *pe)
{
    return shmem_long_fetch (target, *pe);
}

float
FORTRANIFY (shmem_real4_fetch) (float *target, int *pe)
{
    return shmem_float_fetch (target, *pe);
}

double
FORTRANIFY (shmem_real8_fetch) (double *target, int *pe)
{
    return shmem_double_fetch (target, *pe);
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int4_set_ = pshmem_int4_set_
#define shmem_int4_set_ pshmem_int4_set_
#pragma weak shmem_int8_set_ = pshmem_int8_set_
#define shmem_int8_set_ pshmem_int8_set_
#pragma weak shmem_real4_set_ = pshmem_real4_set_
#define shmem_real4_set_ pshmem_real4_set_
#pragma weak shmem_real8_set_ = pshmem_real8_set_
#define shmem_real8_set_ pshmem_real8_set_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_int4_set) (int *target, int *val, int *pe)
{
    shmem_int_set (target, *val, *pe);
}

void
FORTRANIFY (shmem_int8_set) (long *target, long *val, int *pe)
{
    shmem_long_set (target, *val, *pe);
}

void
FORTRANIFY (shmem_real4_set) (float *target, float *val, int *pe)
{
    shmem_float_set (target, *val, *pe);
}

void
FORTRANIFY (shmem_real8_set) (double *target, double *val, int *pe)
{
    shmem_double_set (target, *val, *pe);
}

#if defined(HAVE_FEATURE_EXPERIMENTAL)

/*
 * atomic xor
 *
 * Proposed by IBM Zurich
 *
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmemx_int4_xor_ = pshmemx_int4_xor_
#define shmemx_int4_xor_ pshmemx_int4_xor_
#pragma weak shmemx_int8_xor_ = pshmemx_int8_xor_
#define shmemx_int8_xor_ pshmemx_int8_xor_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmemx_int4_xor) (int *target, int *value, int *pe)
{
    shmemx_int_xor (target, *value, *pe);
}

void
FORTRANIFY (shmemx_int8_xor) (long *target, long *value, int *pe)
{
    shmemx_long_xor (target, *value, *pe);
}

#endif /* HAVE_FEATURE_EXPERIMENTAL */

/*
 * broadcasts
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_broadcast4_ = pshmem_broadcast4_
#define shmem_broadcast4_ pshmem_broadcast4_
#pragma weak shmem_broadcast8_ = pshmem_broadcast8_
#define shmem_broadcast8_ pshmem_broadcast8_
#pragma weak shmem_broadcast32_ = pshmem_broadcast32_
#define shmem_broadcast32_ pshmem_broadcast32_
#pragma weak shmem_broadcast64_ = pshmem_broadcast64_
#define shmem_broadcast64_ pshmem_broadcast64_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_broadcast4) (void *target, const void *source, int *nelems,
                               int *PE_root, int *PE_start,
                               int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast32 (target, source,
                       *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                       (long *) pSync);
}

void
FORTRANIFY (shmem_broadcast8) (void *target, const void *source, int *nelems,
                               int *PE_root, int *PE_start,
                               int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast64 (target, source,
                       *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                       (long *) pSync);
}

void
FORTRANIFY (shmem_broadcast32) (void *target, const void *source,
                                int *nelems, int *PE_root, int *PE_start,
                                int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast32 (target, source,
                       *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                       (long *) pSync);
}

void
FORTRANIFY (shmem_broadcast64) (void *target, const void *source,
                                int *nelems, int *PE_root, int *PE_start,
                                int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast64 (target, source,
                       *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                       (long *) pSync);
}



/*
 * fixed collects
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_fcollect4_ = pshmem_fcollect4_
#define shmem_fcollect4_ pshmem_fcollect4_
#pragma weak shmem_fcollect8_ = pshmem_fcollect8_
#define shmem_fcollect8_ pshmem_fcollect8_
#pragma weak shmem_fcollect32_ = pshmem_fcollect32_
#define shmem_fcollect32_ pshmem_fcollect32_
#pragma weak shmem_fcollect64_ = pshmem_fcollect64_
#define shmem_fcollect64_ pshmem_fcollect64_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_fcollect32) (void *target, const void *source, int *nelems,
                               int *PE_start, int *logPE_stride, int *PE_size,
                               int *pSync)
{
    shmem_fcollect32 (target, source,
                      *nelems, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY (shmem_fcollect4) (void *target, const void *source, int *nelems,
                              int *PE_start, int *logPE_stride, int *PE_size,
                              int *pSync)
{
    shmem_fcollect32 (target, source,
                      *nelems, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY (shmem_fcollect64) (void *target, const void *source, int *nelems,
                               int *PE_start, int *logPE_stride, int *PE_size,
                               int *pSync)
{
    shmem_fcollect64 (target, source,
                      *nelems, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY (shmem_fcollect8) (void *target, const void *source, int *nelems,
                              int *PE_start, int *logPE_stride, int *PE_size,
                              int *pSync)
{
    shmem_fcollect64 (target, source,
                      *nelems, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}


/*
 * generalized collects
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_collect4_ = pshmem_collect4_
#define shmem_collect4_ pshmem_collect4_
#pragma weak shmem_collect8_ = pshmem_collect8_
#define shmem_collect8_ pshmem_collect8_
#pragma weak shmem_collect32_ = pshmem_collect32_
#define shmem_collect32_ pshmem_collect32_
#pragma weak shmem_collect64_ = pshmem_collect64_
#define shmem_collect64_ pshmem_collect64_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_collect32) (void *target, const void *source, int *nelems,
                              int *PE_start, int *logPE_stride, int *PE_size,
                              int *pSync)
{
    shmem_collect32 (target, source, *nelems,
                     *PE_start, *logPE_stride, *PE_size, (long *) pSync);
}

void
FORTRANIFY (shmem_collect64) (void *target, const void *source, int *nelems,
                              int *PE_start, int *logPE_stride, int *PE_size,
                              int *pSync)
{
    shmem_collect64 (target, source, *nelems,
                     *PE_start, *logPE_stride, *PE_size, (long *) pSync);
}

void
FORTRANIFY (shmem_collect4) (void *target, const void *source, int *nelems,
                             int *PE_start, int *logPE_stride, int *PE_size,
                             int *pSync)
{
    shmem_collect32 (target, source, *nelems,
                     *PE_start, *logPE_stride, *PE_size, (long *) pSync);
}

void
FORTRANIFY (shmem_collect8) (void *target, const void *source, int *nelems,
                             int *PE_start, int *logPE_stride, int *PE_size,
                             int *pSync)
{
    shmem_collect64 (target, source, *nelems,
                     *PE_start, *logPE_stride, *PE_size, (long *) pSync);
}


/*
 * reductions
 *
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_int2_sum_to_all_ = pshmem_int2_sum_to_all_
#define shmem_int2_sum_to_all_ pshmem_int2_sum_to_all_
#pragma weak shmem_int4_sum_to_all_ = pshmem_int4_sum_to_all_
#define shmem_int4_sum_to_all_ pshmem_int4_sum_to_all_
#pragma weak shmem_int8_sum_to_all_ = pshmem_int8_sum_to_all_
#define shmem_int8_sum_to_all_ pshmem_int8_sum_to_all_
#pragma weak shmem_real4_sum_to_all_ = pshmem_real4_sum_to_all_
#define shmem_real4_sum_to_all_ pshmem_real4_sum_to_all_
#pragma weak shmem_real8_sum_to_all_ = pshmem_real8_sum_to_all_
#define shmem_real8_sum_to_all_ pshmem_real8_sum_to_all_
#pragma weak shmem_real16_sum_to_all_ = pshmem_real16_sum_to_all_
#define shmem_real16_sum_to_all_ pshmem_real16_sum_to_all_
#pragma weak shmem_int2_prod_to_all_ = pshmem_int2_prod_to_all_
#define shmem_int2_prod_to_all_ pshmem_int2_prod_to_all_
#pragma weak shmem_int4_prod_to_all_ = pshmem_int4_prod_to_all_
#define shmem_int4_prod_to_all_ pshmem_int4_prod_to_all_
#pragma weak shmem_int8_prod_to_all_ = pshmem_int8_prod_to_all_
#define shmem_int8_prod_to_all_ pshmem_int8_prod_to_all_
#pragma weak shmem_real4_prod_to_all_ = pshmem_real4_prod_to_all_
#define shmem_real4_prod_to_all_ pshmem_real4_prod_to_all_
#pragma weak shmem_real8_prod_to_all_ = pshmem_real8_prod_to_all_
#define shmem_real8_prod_to_all_ pshmem_real8_prod_to_all_
#pragma weak shmem_real16_prod_to_all_ = pshmem_real16_prod_to_all_
#define shmem_real16_prod_to_all_ pshmem_real16_prod_to_all_
#pragma weak shmem_int2_max_to_all_ = pshmem_int2_max_to_all_
#define shmem_int2_max_to_all_ pshmem_int2_max_to_all_
#pragma weak shmem_int4_max_to_all_ = pshmem_int4_max_to_all_
#define shmem_int4_max_to_all_ pshmem_int4_max_to_all_
#pragma weak shmem_int8_max_to_all_ = pshmem_int8_max_to_all_
#define shmem_int8_max_to_all_ pshmem_int8_max_to_all_
#pragma weak shmem_real4_max_to_all_ = pshmem_real4_max_to_all_
#define shmem_real4_max_to_all_ pshmem_real4_max_to_all_
#pragma weak shmem_real8_max_to_all_ = pshmem_real8_max_to_all_
#define shmem_real8_max_to_all_ pshmem_real8_max_to_all_
#pragma weak shmem_real16_max_to_all_ = pshmem_real16_max_to_all_
#define shmem_real16_max_to_all_ pshmem_real16_max_to_all_
#pragma weak shmem_int2_min_to_all_ = pshmem_int2_min_to_all_
#define shmem_int2_min_to_all_ pshmem_int2_min_to_all_
#pragma weak shmem_int4_min_to_all_ = pshmem_int4_min_to_all_
#define shmem_int4_min_to_all_ pshmem_int4_min_to_all_
#pragma weak shmem_int8_min_to_all_ = pshmem_int8_min_to_all_
#define shmem_int8_min_to_all_ pshmem_int8_min_to_all_
#pragma weak shmem_real4_min_to_all_ = pshmem_real4_min_to_all_
#define shmem_real4_min_to_all_ pshmem_real4_min_to_all_
#pragma weak shmem_real8_min_to_all_ = pshmem_real8_min_to_all_
#define shmem_real8_min_to_all_ pshmem_real8_min_to_all_
#pragma weak shmem_real16_min_to_all_ = pshmem_real16_min_to_all_
#define shmem_real16_min_to_all_ pshmem_real16_min_to_all_
#pragma weak shmem_int2_and_to_all_ = pshmem_int2_and_to_all_
#define shmem_int2_and_to_all_ pshmem_int2_and_to_all_
#pragma weak shmem_int4_and_to_all_ = pshmem_int4_and_to_all_
#define shmem_int4_and_to_all_ pshmem_int4_and_to_all_
#pragma weak shmem_int8_and_to_all_ = pshmem_int8_and_to_all_
#define shmem_int8_and_to_all_ pshmem_int8_and_to_all_
#pragma weak shmem_int4_or_to_all_ = pshmem_int4_or_to_all_
#define shmem_int2_or_to_all_ pshmem_int2_or_to_all_
#pragma weak shmem_int2_or_to_all_ = pshmem_int2_or_to_all_
#define shmem_int4_or_to_all_ pshmem_int4_or_to_all_
#pragma weak shmem_int8_or_to_all_ = pshmem_int8_or_to_all_
#define shmem_int8_or_to_all_ pshmem_int8_or_to_all_
#pragma weak shmem_int2_xor_to_all_ = pshmem_int2_xor_to_all_
#define shmem_int2_xor_to_all_ pshmem_int2_xor_to_all_
#pragma weak shmem_int4_xor_to_all_ = pshmem_int4_xor_to_all_
#define shmem_int4_xor_to_all_ pshmem_int4_xor_to_all_
#pragma weak shmem_int8_xor_to_all_ = pshmem_int8_xor_to_all_
#define shmem_int8_xor_to_all_ pshmem_int8_xor_to_all_
#pragma weak shmem_comp4_sum_to_all_ = pshmem_comp4_sum_to_all_
#define shmem_comp4_sum_to_all_ pshmem_comp4_sum_to_all_
#pragma weak shmem_comp8_sum_to_all_ = pshmem_comp8_sum_to_all_
#define shmem_comp8_sum_to_all_ pshmem_comp8_sum_to_all_
#pragma weak shmem_comp4_prod_to_all_ = pshmem_comp4_prod_to_all_
#define shmem_comp4_prod_to_all_ pshmem_comp4_prod_to_all_
#pragma weak shmem_comp8_prod_to_all_ = pshmem_comp8_prod_to_all_
#define shmem_comp8_prod_to_all_ pshmem_comp8_prod_to_all_
#endif /* HAVE_FEATURE_PSHMEM */

#define REDUCIFY(Op, Fname, Cname, Ctype)                               \
    void                                                                \
    FORTRANIFY(shmem_##Fname##_##Op##_to_all)                           \
         (Ctype *target, Ctype *source, int *nreduce,                   \
          int *PE_start, int *logPE_stride, int *PE_size,               \
          Ctype *pWrk,                                                  \
          int *pSync)                                                   \
    {                                                                   \
        shmem_##Cname##_##Op##_to_all (target, source,                  \
                                       *nreduce, *PE_start,             \
                                       *logPE_stride, *PE_size,         \
                                       pWrk,                            \
                                       (long *) pSync);                 \
    }

REDUCIFY (sum, int2, short, short);
REDUCIFY (sum, int4, int, int);
REDUCIFY (sum, int8, long, long);
REDUCIFY (sum, real4, float, float);
REDUCIFY (sum, real8, double, double);
REDUCIFY (sum, real16, longdouble, long double);
REDUCIFY (prod, int2, short, short);
REDUCIFY (prod, int4, int, int);
REDUCIFY (prod, int8, long, long);
REDUCIFY (prod, real4, float, float);
REDUCIFY (prod, real8, double, double);
REDUCIFY (prod, real16, longdouble, long double);
REDUCIFY (max, int2, short, short);
REDUCIFY (max, int4, int, int);
REDUCIFY (max, int8, long, long);
REDUCIFY (max, real4, float, float);
REDUCIFY (max, real8, double, double);
REDUCIFY (max, real16, longdouble, long double);
REDUCIFY (min, int2, short, short);
REDUCIFY (min, int4, int, int);
REDUCIFY (min, int8, long, long);
REDUCIFY (min, real4, float, float);
REDUCIFY (min, real8, double, double);
REDUCIFY (min, real16, longdouble, long double);
REDUCIFY (and, int2, short, short);
REDUCIFY (and, int4, int, int);
REDUCIFY (and, int8, long, long);
REDUCIFY (or, int2, short, short);
REDUCIFY (or, int4, int, int);
REDUCIFY (or, int8, long, long);
REDUCIFY (xor, int2, short, short);
REDUCIFY (xor, int4, int, int);
REDUCIFY (xor, int8, long, long);
REDUCIFY (sum, comp4, complexf, float complex);
REDUCIFY (sum, comp8, complexd, double complex);
REDUCIFY (prod, comp4, complexf, float complex);
REDUCIFY (prod, comp8, complexd, double complex);

/*
 * alltoall
 */

void
FORTRANIFY (shmem_alltoall32) (void *target, const void *source, int *nelems,
                               int *PE_start, int *logPE_stride, int *PE_size,
                               int *pSync)
{
    shmem_alltoall32 (target, source, *nelems,
                      *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY (shmem_alltoall64) (void *target, const void *source, int *nelems,
                               int *PE_start, int *logPE_stride, int *PE_size,
                               int *pSync)
{
    shmem_alltoall64 (target, source, *nelems,
                      *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY (shmem_alltoalls32) (void *target, const void *source,
                                ptrdiff_t *dst, ptrdiff_t *sst,
                                int *nelems,
                                int *PE_start, int *logPE_stride, int *PE_size,
                                int *pSync)
{
    shmem_alltoalls32 (target, source,
                       *dst, *sst,
                       *nelems,
                       *PE_start, *logPE_stride, *PE_size,
                       (long *) pSync);
}

void
FORTRANIFY (shmem_alltoalls64) (void *target, const void *source,
                                ptrdiff_t *dst, ptrdiff_t *sst,
                                int *nelems,
                                int *PE_start, int *logPE_stride, int *PE_size,
                                int *pSync)
{
    shmem_alltoalls64 (target, source,
                       *dst, *sst,
                       *nelems,
                       *PE_start, *logPE_stride, *PE_size,
                       (long *) pSync);
}

/*
 * locks
 *
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_clear_lock_ = pshmem_clear_lock_
#define shmem_clear_lock_ pshmem_clear_lock_
#pragma weak shmem_set_lock_ = pshmem_set_lock_
#define shmem_set_lock_ pshmem_set_lock_
#pragma weak shmem_test_lock_ = pshmem_test_lock_
#define shmem_test_lock_ pshmem_test_lock_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_clear_lock) (long *lock)
{
    shmem_clear_lock (lock);
}

void
FORTRANIFY (shmem_set_lock) (long *lock)
{
    shmem_set_lock (lock);
}

int
FORTRANIFY (shmem_test_lock) (long *lock)
{
    return shmem_test_lock (lock);
}

#if defined(HAVE_FEATURE_PSHMEM)

/*
 * proposed profiling interface (note: no pshmem equiv)
 *
 */

extern void shmem_pcontrol (int level);

void
FORTRANIFY (shmem_pcontrol) (int *level)
{
    shmem_pcontrol (*level);
}

#endif /* HAVE_FEATURE_PSHMEM */

#if defined(HAVE_FEATURE_EXPERIMENTAL)

/*
 * WORK IN PROGRESS
 *
 *
 *
 * non-blocking putss
 */

#ifdef HAVE_FEATURE_PSHMEM
/* # pragma weak shmemx_character_put_nb_ = pshmemx_character_put_nb_ */
/* # define shmemx_character_put_nb_ pshmemx_character_put_nb_ */
#pragma weak shmemx_double_put_nb_ = pshmemx_double_put_nb_
#define shmemx_double_put_nb_ pshmemx_double_put_nb_
#pragma weak shmemx_integer_put_nb_ = pshmemx_integer_put_nb_
#define shmemx_integer_put_nb_ pshmemx_integer_put_nb_
#pragma weak shmemx_logical_put_nb_ = pshmemx_logical_put_nb_
#define shmemx_logical_put_nb_ pshmemx_logical_put_nb_
#pragma weak shmemx_real_put_nb_ = pshmemx_real_put_nb_
#define shmemx_real_put_nb_ pshmemx_real_put_nb_
/* # pragma weak shmemx_complex_put_nb_ = pshmemx_complex_put_nb_*/
/* # define shmemx_complex_put_nb_ pshmemx_complex_put_nb_*/
#pragma weak shmemx_put4_nb_ = pshmemx_put4_nb_
#define shmemx_put4_nb_ pshmemx_put4_nb_
#pragma weak shmemx_put8_nb_ = pshmemx_put8_nb_
#define shmemx_put8_nb_ pshmemx_put8_nb_
#pragma weak shmemx_put32_nb_ = pshmemx_put32_nb_
#define shmemx_put32_nb_ pshmemx_put32_nb_
#pragma weak shmemx_put64_nb_ = pshmemx_put64_nb_
#define shmemx_put64_nb_ pshmemx_put64_nb_
#pragma weak shmemx_put128_nb_ = pshmemx_put128_nb_
#define shmemx_put128_nb_ pshmemx_put128_nb_
/* # pragma weak shmemx_putmem_nb_ = pshmemx_putmem_nb_ */
/* # define shmemx_putmem_nb_ pshmemx_putmem_nb_ */
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEMX_FORTRAN_PUT_NB(FName, CName, CType)                      \
    void                                                                \
    FORTRANIFY(shmemx_##FName##_put_nb) (CType *target, const CType *source, \
                                         int *size, int *pe,            \
                                         shmemx_request_handle_t *desc) \
    {                                                                   \
        shmemx_##CName##_put_nb (target, source, *size, *pe, desc);     \
    }

#define SHMEMX_FORTRAN_PUT_SIZE_NB(Size, CName, CType)                  \
    void                                                                \
    FORTRANIFY(shmemx_put##Size##_nb) (CType *target, const CType *source, \
                                       int *size, int *pe,              \
                                       shmemx_request_handle_t *desc)   \
    {                                                                   \
        shmemx_##CName##_put_nb (target, source, *size, *pe, desc);     \
    }

/* SHMEMX_FORTRAN_PUT_NB (character, char, char); */
SHMEMX_FORTRAN_PUT_NB (double, double, double);
SHMEMX_FORTRAN_PUT_NB (integer, int, int);
SHMEMX_FORTRAN_PUT_NB (logical, int, int);
SHMEMX_FORTRAN_PUT_NB (real, int, int);
/* SHMEMX_FORTRAN_PUT_NB (complex, complexf, COMPLEXIFY (float)); */
SHMEMX_FORTRAN_PUT_SIZE_NB (4, int, int);
SHMEMX_FORTRAN_PUT_SIZE_NB (8, long, long);
SHMEMX_FORTRAN_PUT_SIZE_NB (32, int, int);
SHMEMX_FORTRAN_PUT_SIZE_NB (64, long, long);
SHMEMX_FORTRAN_PUT_SIZE_NB (128, longlong, long long);

/*
 * non-blocking gets
 */

#ifdef HAVE_FEATURE_PSHMEM
/* # pragma weak shmemx_character_get_nb_ = pshmemx_character_get_nb_ */
/* # define shmemx_character_get_nb_ pshmemx_character_get_nb_ */
#pragma weak shmemx_double_get_nb_ = pshmemx_double_get_nb_
#define shmemx_double_get_nb_ pshmemx_double_get_nb_
#pragma weak shmemx_integer_get_nb_ = pshmemx_integer_get_nb_
#define shmemx_integer_get_nb_ pshmemx_integer_get_nb_
#pragma weak shmemx_logical_get_nb_ = pshmemx_logical_get_nb_
#define shmemx_logical_get_nb_ pshmemx_logical_get_nb_
#pragma weak shmemx_real_get_nb_ = pshmemx_real_get_nb_
#define shmemx_real_get_nb_ pshmemx_real_get_nb_
/* # pragma weak shmemx_complex_get_nb_ = pshmemx_complex_get_nb_*/
/* # define shmemx_complex_get_nb_ pshmemx_complex_get_nb_*/
#pragma weak shmemx_get4_nb_ = pshmemx_get4_nb_
#define shmemx_get4_nb_ pshmemx_get4_nb_
#pragma weak shmemx_get8_nb_ = pshmemx_get8_nb_
#define shmemx_get8_nb_ pshmemx_get8_nb_
#pragma weak shmemx_get32_nb_ = pshmemx_get32_nb_
#define shmemx_get32_nb_ pshmemx_get32_nb_
#pragma weak shmemx_get64_nb_ = pshmemx_get64_nb_
#define shmemx_get64_nb_ pshmemx_get64_nb_
#pragma weak shmemx_get128_nb_ = pshmemx_get128_nb_
#define shmemx_get128_nb_ pshmemx_get128_nb_
/* # pragma weak shmemx_getmem_nb_ = pshmemx_getmem_nb_ */
/* # define shmemx_getmem_nb_ pshmemx_getmem_nb_ */
#endif /* HAVE_FEATURE_PSHMEM */

#define SHMEMX_FORTRAN_GET_NB(FName, CName, CType)                      \
    void                                                                \
    FORTRANIFY(shmemx_##FName##_get_nb) (CType *target, const CType *source, \
                                         int *size, int *pe,            \
                                         shmemx_request_handle_t *desc) \
    {                                                                   \
        shmemx_##CName##_get_nb (target, source, *size, *pe, desc);     \
    }

#define SHMEMX_FORTRAN_GET_SIZE_NB(Size, CName, CType)                  \
    void                                                                \
    FORTRANIFY(shmemx_get##Size##_nb) (CType *target, const CType *source, \
                                       int *size, int *pe,              \
                                       shmemx_request_handle_t *desc)   \
    {                                                                   \
        shmemx_##CName##_get_nb (target, source, *size, *pe, desc);     \
    }

/* SHMEMX_FORTRAN_GET_NB (character, char, char); */
SHMEMX_FORTRAN_GET_NB (double, double, double);
SHMEMX_FORTRAN_GET_NB (integer, int, int);
SHMEMX_FORTRAN_GET_NB (logical, int, int);
SHMEMX_FORTRAN_GET_NB (real, int, int);
/* SHMEMX_FORTRAN_GET_NB (complex, complexf, COMPLEXIFY (float)); */
SHMEMX_FORTRAN_GET_SIZE_NB (4, int, int);
SHMEMX_FORTRAN_GET_SIZE_NB (8, long, long);
SHMEMX_FORTRAN_GET_SIZE_NB (32, int, int);
SHMEMX_FORTRAN_GET_SIZE_NB (64, long, long);
SHMEMX_FORTRAN_GET_SIZE_NB (128, longlong, long long);

#endif /* HAVE_FEATURE_EXPERIMENTAL */

#if defined(HAVE_FEATURE_EXPERIMENTAL)

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmemx_wtime_ = pshmemx_wtime_
#define shmemx_wtime_ pshmemx_wtime_
#endif /* HAVE_FEATURE_PSHMEM */

double
FORTRANIFY (shmemx_wtime) (void)
{
    return shmemx_wtime ();
}

#endif /* HAVE_FEATURE_EXPERIMENTAL */

#if defined(HAVE_FEATURE_EXPERIMENTAL)

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmemx_lookup_remote_addr_ = pshmemx_lookup_remote_addr_
#define shmemx_lookup_remote_addr_ pshmemx_lookup_remote_addr_
#endif /* HAVE_FEATURE_PSHMEM */

uintptr_t *
FORTRANIFY (shmemx_lookup_remote_addr) (uintptr_t *addr, int *pe)
{
    return shmemx_lookup_remote_addr (addr, *pe);
}

#endif /* HAVE_FEATURE_EXPERIMENTAL */


#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_info_get_version_ = pshmem_info_get_version_
#define shmem_info_get_version_ pshmem_info_get_version_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_info_get_version) (int *major, int *minor)
{
    shmem_info_get_version (major, minor);
}


#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_info_get_name_ = pshmem_info_get_name_
#define shmem_info_get_name_ pshmem_info_get_name_
#endif /* HAVE_FEATURE_PSHMEM */

void
FORTRANIFY (shmem_info_get_name) (char *name)
{
    int len;
    const int max_f_len = SHMEM_MAX_NAME_LEN - 1;

    /* get the C string */
    shmem_info_get_name (name);

    /* pad with spaces for Fortran (TODO: memset better?) */
    len = strlen (name);
    while (len < max_f_len) {
        name[len] = ' ';
        len += 1;
    }
}

#if defined(HAVE_FEATURE_EXPERIMENTAL)

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmemx_fence_test_ = pshmemx_fence_test_
#define shmemx_fence_test_ pshmemx_fence_test_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmemx_fence_test) (void)
{
    return shmemx_fence_test ();
}

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmemx_quiet_test_ = pshmemx_quiet_test_
#define shmemx_quiet_test_ pshmemx_quiet_test_
#endif /* HAVE_FEATURE_PSHMEM */

int
FORTRANIFY (shmemx_quiet_test) (void)
{
    return shmemx_quiet_test ();
}

#endif /* HAVE_FEATURE_EXPERIMENTAL */


/**
 *
 * DEPRECATED API
 *
 * cache flushing
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shmem_clear_cache_inv_ = pshmem_clear_cache_inv_
#define shmem_clear_cache_inv_ pshmem_clear_cache_inv_
#pragma weak shmem_clear_cache_line_inv_ = pshmem_clear_cache_line_inv_
#define shmem_clear_cache_line_inv_ pshmem_clear_cache_line_inv_
#pragma weak shmem_set_cache_inv_ = pshmem_set_cache_inv_
#define shmem_set_cache_inv_ pshmem_set_cache_inv_
#pragma weak shmem_set_cache_line_inv_ = pshmem_set_cache_line_inv_
#define shmem_set_cache_line_inv_ pshmem_set_cache_line_inv_
#pragma weak shmem_udcflush_line_ = pshmem_udcflush_line_
#define shmem_udcflush_line_ pshmem_udcflush_line_
#pragma weak shmem_udcflush_ = pshmem_udcflush_
#define shmem_udcflush_ pshmem_udcflush_
#endif /* HAVE_FEATURE_PSHMEM */


FORTRANIFY_VOID_VOID (shmem_clear_cache_inv);
FORTRANIFY_VOID_VOID (shmem_set_cache_inv);
FORTRANIFY_VOID_VOID (shmem_udcflush);

#define FORTRANIFY_CACHE(Name)                  \
    void                                        \
    FORTRANIFY(Name) (void *target)             \
    {                                           \
        Name (target);                          \
    }

FORTRANIFY_CACHE (shmem_set_cache_line_inv);
FORTRANIFY_CACHE (shmem_clear_cache_line_inv);
FORTRANIFY_CACHE (shmem_udcflush_line);
