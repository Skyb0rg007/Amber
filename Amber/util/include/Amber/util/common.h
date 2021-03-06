/**
 * @file common.h
 * @brief Common C programming utilities
 */
#ifndef AMBER_UTIL_COMMON_H
#define AMBER_UTIL_COMMON_H

#include <Amber/compat/stdbool.h>
#include <Amber/compat/stdint.h>
#include <Amber/compat/inttypes.h>
#include <Amber/compat/compat.h>
#include <stdlib.h>
#include <SDL_assert.h>
#include <SDL_log.h>
#include <SDL_error.h>

/************************************************
 * Convenience Macros
 ************************************************/
/** @brief Get the size of an array */
#if AB_COMPILER_IS_GNU || AB_COMPILER_IS_Clang || AB_COMPILER_IS_AppleClang
# define AB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + \
        sizeof(__typeof__(int[1 - 2 * \
                __builtin_types_compatible_p(__typeof__(arr), \
                    __typeof__(&arr[0]))])) * 0)
#elif defined(__countof) /* MSVC */
# define AB_ARRAY_SIZE(arr) __countof(arr)
#else
# define AB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif
/** @brief Convert radians to degrees */
#define AB_DEGREES(X) ((X) * 180.0 / PI)
/** @brief Convert degrees to radians */
#define AB_RADIANS(X) ((X) * PI / 180.0)
/** @brief The max of two values */
#define AB_MAX(X, Y) ((X) > (Y) ? (X) : (Y))
/** @brief The min of two values */
#define AB_MIN(X, Y) ((X) > (Y) ? (Y) : (X))
/** @brief Clamps the second argument between the first and last */
#define AB_CLAMP(X, Y, Z) MIN(MAX(X, Y), Z)
/** @brief The value pi */
#define AB_PI 3.14159265359

/************************************************
 * Logging and Assertion Macros
 ************************************************/
/** @brief Log levels */
enum {
    AB_LOG_PRIORITY_VERBOSE  = SDL_LOG_PRIORITY_VERBOSE,
    AB_LOG_PRIORITY_DEBUG    = SDL_LOG_PRIORITY_DEBUG,
    AB_LOG_PRIORITY_INFO     = SDL_LOG_PRIORITY_INFO,
    AB_LOG_PRIORITY_WARN     = SDL_LOG_PRIORITY_WARN,
    AB_LOG_PRIORITY_ERROR    = SDL_LOG_PRIORITY_ERROR,
    AB_LOG_PRIORITY_CRITICAL = SDL_LOG_PRIORITY_CRITICAL
};

/** @brief Log categories */
enum {
    AB_LOG_CATEGORY_GENERAL = SDL_LOG_CATEGORY_APPLICATION,
    AB_LOG_CATEGORY_UTIL = SDL_LOG_CATEGORY_CUSTOM,
    AB_LOG_CATEGORY_MAX
};

/** @brief Log a message */
#define AB_LOG  SDL_LogMessage
/** @brief Log a message using a va_list */
#define AB_LOGV SDL_LogMessageV

/** @brief Quick and dirty logging: for tests + quickly debugging, not prod */
#define AB_QUICK_LOG(...) AB_LOG(AB_LOG_CATEGORY_GENERAL, AB_LOG_PRIORITY_INFO, __VA_ARGS__)

/** @brief The assert expression function */
#define AB_assert_expr_func(cond, str, file, line) \
    (!(cond) ? (AB_LOG(AB_LOG_CATEGORY_GENERAL, AB_LOG_PRIORITY_CRITICAL, "%s[%d]: %s", __FILE__, __LINE__, #cond), abort(), 0) : 0)

/** @brief General assertion */
#define AB_ASSERT(X)          SDL_assert(X)
/** @brief Assertion macro that's always an expression */
#ifndef NDEBUG
# define AB_ASSERT_EXPR(X)     AB_assert_expr_func(X, #X, __FILE__, __LINE__)
#else
# define AB_ASSERT_EXPR(X)     (void)0
#endif
/** @brief Assertion that remains in release builds */
#define AB_ASSERT_RELEASE(X)  SDL_assert_release(X)
/** @brief Assertion that is only enabled if desired */
#define AB_ASSERT_PARANOID(X) SDL_assert_paranoid(X)

/** @brief Change the logging level 
 * @param X The log priority
 *
 * Possible values:
 * - AB_LOG_PRIORITY_VERBOSE
 * - AB_LOG_PRIORITY_DEBUG
 * - AB_LOG_PRIORITY_INFO
 * - AB_LOG_PRIORITY_WARN
 * - AB_LOG_PRIORITY_ERROR
 * - AB_LOG_PRIORITY_CRITICAL
 */
#define AB_LOG_SETLEVEL(cat, lvl) SDL_LogSetPriority(cat, lvl)
#define AB_LOG_GETLEVEL(cat)      SDL_LogGetPriority(cat)

/** @brief Compile-time assertion *expression*
 * Evaluates to 0 at runtime 
 */
#define AB_STATIC_ASSERT(cond) (sizeof(char[1 - 2 * !(cond)]) * 0)

/************************************************
 * Error handling
 ************************************************/
typedef enum {
    AB_OK = 0,
    AB_ENOMEM,  /* Out of memory */
    AB_EACCESS, /* Cannot access file */
    AB_EXIT,    /* User decided to exit */
    AB_ESDL,    /* SDL-based error */
    AB_ERRMSG   /* Error with message set */
} AB_errno_t;

/** @brief Set the current error message
 * You should return AB_ERRMSG after doing this */
#define AB_seterr SDL_SetError
/** @brief Get a string representation of an error code */
const char *AB_strerror(AB_errno_t err);

#endif /* AMBER_UTIL_COMMON_H */
