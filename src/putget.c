#include <stdio.h>               /* NULL                           */
#include <string.h>              /* memcpy()                       */
#include <sys/types.h>           /* size_t                         */

#include "state.h"
#include "symmem.h"
#include "comms.h"
#include "updown.h"
#include "warn.h"

/*
 * short-circuit local puts/gets, otherwise translate between
 * local/remote addresses
 * (should probably ifdef for aligned segments case)
 */

#define SHMEM_TYPE_PUT(Name, Type)					\
  /* @api@ */								\
  void									\
  shmem_##Name##_put(Type *dest, const Type *src, size_t len, int pe)	\
  {									\
    int typed_len = sizeof(Type) * len;					\
    if (__state.mype == pe) {						\
      memcpy(dest, src, typed_len);					\
    }									\
    else {								\
      void *rdest = __symmetric_addr_lookup(dest, pe);			\
      if (rdest == NULL) {						\
	__shmem_warn(SHMEM_LOG_FATAL,					\
		     "shmem_%s_put: address at %p is not symmetric", \
		     #Name,						\
		     dest						\
		     );							\
	/* NOT REACHED */						\
      }									\
      __comms_put(rdest, (Type *) src, typed_len, pe);			\
    }									\
  }

SHMEM_TYPE_PUT(short, short)
SHMEM_TYPE_PUT(int, int)
SHMEM_TYPE_PUT(long, long)
SHMEM_TYPE_PUT(longlong, long long)
SHMEM_TYPE_PUT(longdouble, long double)
SHMEM_TYPE_PUT(double, double)
SHMEM_TYPE_PUT(float, float)

#pragma weak shmem_putmem = shmem_long_put
#pragma weak shmem_put = shmem_long_put
#pragma weak shmem_put32 = shmem_int_put
#pragma weak shmem_put64 = shmem_long_put
#pragma weak shmem_put128 = shmem_longdouble_put

#ifdef HAVE_PSHMEM_SUPPORT
#pragma weak pshmem_short_put = shmem_short_put
#pragma weak pshmem_int_put = shmem_int_put
#pragma weak pshmem_long_put = shmem_long_put
#pragma weak pshmem_longdouble_put = shmem_longdouble_put
#pragma weak pshmem_longlong_put = shmem_longlong_put
#pragma weak pshmem_double_put = shmem_double_put
#pragma weak pshmem_float_put = shmem_float_put
#pragma weak pshmem_putmem = shmem_putmem
#pragma weak pshmem_put = shmem_put
#pragma weak pshmem_put32 = shmem_put32
#pragma weak pshmem_put64 = shmem_put64
#pragma weak pshmem_put128 = shmem_put128
#endif /* HAVE_PSHMEM_SUPPORT */

#define SHMEM_TYPE_GET(Name, Type)					\
  /* @api@ */								\
  void									\
  shmem_##Name##_get(Type *dest, const Type *src, size_t len, int pe)	\
  {									\
    int typed_len = sizeof(Type) * len;					\
    if (__state.mype == pe) {						\
      memcpy(dest, src, typed_len);					\
    }									\
    else {								\
      void *their_src = __symmetric_addr_lookup((Type *) src, pe);	\
      if (their_src == NULL) {						\
	__shmem_warn(SHMEM_LOG_FATAL,					\
		     "shmem_%s_get: address at %p is not symmetric", \
		     #Name,						\
		     src						\
		     );							\
	/* NOT REACHED */						\
      }									\
      __comms_get(dest, their_src, typed_len, pe);			\
    }									\
  }

SHMEM_TYPE_GET(short, short)
SHMEM_TYPE_GET(int, int)
SHMEM_TYPE_GET(long, long)
SHMEM_TYPE_GET(longdouble, long double)
SHMEM_TYPE_GET(longlong, long long)
SHMEM_TYPE_GET(double, double)
SHMEM_TYPE_GET(float, float)

#pragma weak shmem_getmem = shmem_long_get
#pragma weak shmem_get = shmem_long_get
#pragma weak shmem_get32 = shmem_int_get
#pragma weak shmem_get64 = shmem_long_get
#pragma weak shmem_get128 = shmem_longdouble_get

#ifdef HAVE_PSHMEM_SUPPORT
#pragma weak pshmem_short_get = shmem_short_get
#pragma weak pshmem_int_get = shmem_int_get
#pragma weak pshmem_long_get = shmem_long_get
#pragma weak pshmem_longdouble_get = shmem_longdouble_get
#pragma weak pshmem_longlong_get = shmem_longlong_get
#pragma weak pshmem_double_get = shmem_double_get
#pragma weak pshmem_float_get = shmem_float_get
#pragma weak pshmem_getmem = shmem_getmem
#pragma weak pshmem_get = shmem_get
#pragma weak pshmem_get32 = shmem_get32
#pragma weak pshmem_get64 = shmem_get64
#pragma weak pshmem_get128 = shmem_get128
#endif /* HAVE_PSHMEM_SUPPORT */


/*
 * gasnet_(get|get)_val can't handle bigger types..
 */

#define SHMEM_TYPE_P_WRAPPER(Name, Type)				\
  /* @api@ */								\
  void									\
  shmem_##Name##_p(Type *dest, Type value, int pe)			\
  {									\
    shmem_##Name##_put(dest, &value, sizeof(value), pe);		\
  }

SHMEM_TYPE_P_WRAPPER(float, float)
SHMEM_TYPE_P_WRAPPER(double, double)
SHMEM_TYPE_P_WRAPPER(longdouble, long double)
SHMEM_TYPE_P_WRAPPER(longlong, long long)

#define SHMEM_TYPE_P(Name, Type)					\
  /* @api@ */								\
  void									\
  shmem_##Name##_p(Type *dest, Type value, int pe)			\
  {									\
    if (__state.mype == pe) {						\
      *dest = value;							\
    }									\
    else {								\
      void *rdest = __symmetric_addr_lookup(dest, pe);			\
      if (rdest == NULL) {						\
	__shmem_warn(SHMEM_LOG_FATAL,					\
		     "shmem_%s_p: address at %p is not symmetric", \
		     #Name,						\
		     dest						\
		     );							\
	/* NOT REACHED */						\
      }									\
      __comms_put_val(rdest, value, sizeof(Type), pe);			\
    }									\
  }

SHMEM_TYPE_P(short, short)
SHMEM_TYPE_P(int, int)
SHMEM_TYPE_P(long, long)

#ifdef HAVE_PSHMEM_SUPPORT
#pragma weak pshmem_short_p = shmem_short_p
#pragma weak pshmem_int_p = shmem_int_p
#pragma weak pshmem_long_p = shmem_long_p
#pragma weak pshmem_longdouble_p = shmem_longdouble_p
#pragma weak pshmem_longlong_p = shmem_longlong_p
#pragma weak pshmem_double_p = shmem_double_p
#pragma weak pshmem_float_p = shmem_float_p
#endif /* HAVE_PSHMEM_SUPPORT */

#define SHMEM_TYPE_G_WRAPPER(Name, Type)				\
  /* @api@ */								\
  Type									\
  shmem_##Name##_g(Type *dest, int pe)					\
  {									\
    Type retval;							\
    shmem_##Name##_get(dest, &retval, sizeof(retval), pe);		\
    return retval;							\
  }

SHMEM_TYPE_G_WRAPPER(float, float)
SHMEM_TYPE_G_WRAPPER(double, double)
SHMEM_TYPE_G_WRAPPER(longlong, long long)
SHMEM_TYPE_G_WRAPPER(longdouble, long double)

#define SHMEM_TYPE_G(Name, Type)					\
  /* @api@ */								\
  Type									\
  shmem_##Name##_g(Type *src, int pe)					\
  {									\
    Type retval;							\
    if (__state.mype == pe) {						\
      retval = *src;							\
    }									\
    else {								\
      void *their_src = __symmetric_addr_lookup(src, pe);		\
      if (their_src == NULL) {						\
	__shmem_warn(SHMEM_LOG_FATAL,					\
		     "shmem_%s_g: address at %p is not symmetric", \
		     #Name,						\
		     src						\
		     );							\
	/* NOT REACHED */						\
      }									\
      retval = (Type) __comms_get_val(their_src, sizeof(retval), pe);	\
    }									\
    return retval;							\
  }

SHMEM_TYPE_G(short, short)
SHMEM_TYPE_G(int, int)
SHMEM_TYPE_G(long, long)

#ifdef HAVE_PSHMEM_SUPPORT
#pragma weak pshmem_short_g = shmem_short_g
#pragma weak pshmem_int_g = shmem_int_g
#pragma weak pshmem_long_g = shmem_long_g
#pragma weak pshmem_longdouble_g = shmem_longdouble_g
#pragma weak pshmem_longlong_g = shmem_longlong_g
#pragma weak pshmem_double_g = shmem_double_g
#pragma weak pshmem_float_g = shmem_float_g
#endif /* HAVE_PSHMEM_SUPPORT */

/*
 * non-blocking extensions
 */

#define SHMEM_TYPE_PUT_NB(Name, Type)					\
  /* @api@ */								\
  void *								\
  shmem_##Name##_put_nb(Type *target, Type *source, size_t len, int pe)	\
  {									\
    return __comms_##Name##_put_nb(target, source, len, pe);		\
  }

SHMEM_TYPE_PUT_NB(short, short)
SHMEM_TYPE_PUT_NB(int, int)
SHMEM_TYPE_PUT_NB(long, long)
SHMEM_TYPE_PUT_NB(longdouble, long double)
SHMEM_TYPE_PUT_NB(longlong, long long)
SHMEM_TYPE_PUT_NB(double, double)
SHMEM_TYPE_PUT_NB(float, float)

#ifdef HAVE_PSHMEM_SUPPORT
#pragma weak pshmem_short_put_nb = shmem_short_put_nb
#pragma weak pshmem_int_put_nb = shmem_int_put_nb
#pragma weak pshmem_long_put_nb = shmem_long_put_nb
#pragma weak pshmem_longdouble_put_nb = shmem_longdouble_put_nb
#pragma weak pshmem_longlong_put_nb = shmem_longlong_put_nb
#pragma weak pshmem_double_put_nb = shmem_double_g
#pragma weak pshmem_float_put_nb = shmem_float_put_nb
#endif /* HAVE_PSHMEM_SUPPORT */
