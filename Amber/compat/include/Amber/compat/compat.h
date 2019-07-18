/*
 * Macro definitions based on the compiler
 */

#ifndef AMBER_COMPAT_COMPAT_H
#define AMBER_COMPAT_COMPAT_H

#include <Amber/compat/compiler.h>

/* restrict */
#if defined(AB_COMPILER_C_RESTRICT) && AB_COMPILER_C_RESTRICT
# define restrict restrict
#elif AB_COMPILER_IS_MSVC
# define restrict __restrict
#else
# define restrict
#endif

/* inline */
#if __STDC_VERSION__ >= 199901L
# define inline inline
#elif __GNUC__
# define inline __inline__
#elif AB_COMPILER_IS_MSVC
# define inline __inline
#else
# define inline
#endif

/* Attributes */
#if __GNUC__

# define AB_ATTR_ALWAYS_INLINE     __attribute__((__always_inline__)) inline
# define AB_ATTR_NEVER_INLINE      __attribute__((__noinline__))
# define AB_ATTR_CONST             __attribute__((__const__))
# define AB_ATTR_PURE              __attribute__((__pure__))
# define AB_ATTR_DEPRECIATED       __attribute__((__depreciated__))
# define AB_ATTR_NORETURN          __attribute__((__noreturn__))
# define AB_ATTR_PRINTF(archetype, str_idx, first_checked) \
    __attribute__((__format__(archetype, str_idx, first_checked)))

#elif AB_COMPILER_IS_MSVC

# define AB_ATTR_ALWAYS_INLINE     __forceinline inline
# define AB_ATTR_NEVER_INLINE      __declspec(noinline)
# define AB_ATTR_CONST             __declspec(noalias)
# define AB_ATTR_PURE              __declspec(noalias)
# define AB_ATTR_DEPRECIATED
# define AB_ATTR_NORETURN          __declspec(noreturn)
# define AB_ATTR_PRINTF(a, b, c)

#else

# define AB_ATTR_ALWAYS_INLINE     inline
# define AB_ATTR_NEVER_INLINE 
# define AB_ATTR_CONST
# define AB_ATTR_PURE
# define AB_ATTR_DEPRECIATED
# define AB_ATTR_NORETURN
# define AB_ATTR_PRINTF(a, b, c)

#endif

/* visibility */
#if defined(__ELF__) && __GNUC__
# define AB_EXPORT extern
# define AB_LIB    __attribute__((__visibility__("hidden"))) extern
#elif (_WIN32 || __CYGWIN__) && AB_BUILDING_SHARED
# ifdef AB_COMPILING
#  define AB_EXPORT __declspec(dllexport) extern
# else
#  define AB_EXPORT __declspec(dllimport) extern
# endif
# define AB_LIB extern
#else
# define AB_EXPORT extern
# define AB_LIB    extern
#endif

#endif /* AMBER_COMPAT_COMPAT_H */
