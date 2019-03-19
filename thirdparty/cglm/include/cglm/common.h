/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef cglm_common_h
#define cglm_common_h

#define _USE_MATH_DEFINES /* for windows */

#include <stdint.h>
#include <math.h>
#include <float.h>

#if defined(_MSC_VER)
#  ifdef CGLM_DLL
#    define CGLM_EXPORT __declspec(dllexport)
#  else
#    define CGLM_EXPORT __declspec(dllimport)
#  endif
#  define CGLM_INLINE __forceinline
#  define CGLM_RESTRICT __restrict
#elif defined(__GNUC__)
#  define CGLM_INLINE static __inline__ __attribute__((__always_inline__))
#  define CGLM_EXPORT __attribute__((__visibility__("default")))
#  define CGLM_RESTRICT __restrict
#else
#  define CGLM_EXPORT extern
#  if __STDC_VERSION__ >= 199901L
#    define CGLM_INLINE static inline
#    define CGLM_RESTRICT restrict
#  else
#    define CGLM_INLINE static
#    define CGLM_RESTRICT
#  endif
#endif

#define glm__memcpy(type, dest, src, size)                                    \
  do {                                                                        \
    type *srci;                                                               \
    type *srci_end;                                                           \
    type *desti;                                                              \
                                                                              \
    srci     = (type *)src;                                                   \
    srci_end = (type *)((char *)srci + size);                                 \
    desti    = (type *)dest;                                                  \
                                                                              \
    while (srci != srci_end)                                                  \
      *desti++ = *srci++;                                                     \
  } while (0)

#define glm__memset(type, dest, size, val)                                    \
  do {                                                                        \
    type *desti;                                                              \
    type *desti_end;                                                          \
                                                                              \
    desti     = (type *)dest;                                                 \
    desti_end = (type *)((char *)desti + size);                               \
                                                                              \
    while (desti != desti_end)                                                \
      *desti++ = val;                                                         \
  } while (0)

#define glm__memzero(type, dest, size) glm__memset(type, dest, size, 0)

#include "types.h"
#include "simd/intrin.h"

#endif /* cglm_common_h */
