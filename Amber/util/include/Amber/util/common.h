/**
 * @file common.h
 * @brief Common C programming utilities
 */
#ifndef AMBER_UTIL_COMMON_H
#define AMBER_UTIL_COMMON_H

#include <Amber/util/compat.h>
#include <SDL_assert.h>
#include <SDL_log.h>
#include <assert.h>

/************************************************
 * Convenience Macros
 ************************************************/
/** @brief Get the size of an array */
#if AB_COMPILER_IS_GNU || AB_COMPILER_IS_Clang || AB_COMPILER_IS_AppleClang
/* Type-safe ARRAY_SIZE macro */
# define AB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + \
        sizeof(__typeof__(int[1 - 2 * \
                __builtin_types_compatible_p(__typeof__(arr), \
                    __typeof__(&arr[0]))])) * 0)
#else
# define AB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif
/** @brief Convert radians to degrees */
#define AB_DEGREES(X) ((X) * 180.0 / PI)
/** @brief Convert degrees to radians */
#define AB_RADIANS(X) ((X) * PI / 180.0)
/** @brief The max of two values */
#define AB_MAX(X, Y) (X > Y ? X : Y)
/** @brief The min of two values */
#define AB_MIN(X, Y) (X > Y ? Y : X)
/** @brief Clamps the second argument between the first and last */
#define AB_CLAMP(X, Y, Z) MIN(MAX(X, Y), Z)
/** @brief The value pi */
#define AB_PI 3.14159265359

/************************************************
 * Logging and Assertion Macros
 ************************************************/
/** @brief Log arbitrary information */
#define AB_LOG_INFO(...)     SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_INFO, __VA_ARGS__)
/** @brief Log information relating to debugging */
#define AB_LOG_DEBUG(...)    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_DEBUG, __VA_ARGS__)
/** @brief Log a warning - something went wrong but recoverable */
#define AB_LOG_WARN(...)     SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_WARN, __VA_ARGS__)
/** @brief Log an error - something went wrong that shouldn't */
#define AB_LOG_ERROR(...)    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_ERROR, __VA_ARGS__)
/** @brief Log a critical error - not reoverable */
#define AB_LOG_CRITICAL(...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_CRITICAL, __VA_ARGS__)

/** @brief General assertion */
#define AB_ASSERT(X)          SDL_assert(X)
/** @brief Assertion that remains in release builds */
#define AB_ASSERT_RELEASE(X)  SDL_assert_release(X)
/** @brief Assertion that is only enabled if desired */
#define AB_ASSERT_PARANOID(X) SDL_assert_paranoid(X)

/** @brief Change the logging level 
 * @param X The log priority
 * Possible values:
 *   SDL_LOG_PRIORITY_VERBOSE
 *   SDL_LOG_PRIORITY_DEBUG
 *   SDL_LOG_PRIORITY_INFO
 *   SDL_LOG_PRIORITY_WARN
 *   SDL_LOG_PRIORITY_ERROR
 *   SDL_LOG_PRIORITY_CRITICAL
 */
#define AB_LOG_SETLEVEL(X) SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, X)

/** @brief Compile-time assertion expression
 * Evaluates to 0 on runtime 
 */
#define AB_STATIC_ASSERT(cond) (sizeof(char[1 - 2 * !(cond)]) * 0)

#endif /* AMBER_UTIL_COMMON_H */
