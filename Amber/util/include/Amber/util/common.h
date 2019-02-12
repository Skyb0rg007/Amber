#ifndef AMBER_UTIL_COMMON_H
#define AMBER_UTIL_COMMON_H

#include <Amber/util/compat.h>
#include <SDL_assert.h>
#include <SDL_log.h>
#include <assert.h>

/************************************************
 * Convenience Macros
 ************************************************/
#if AB_COMPILER_IS_GNU || AB_COMPILER_IS_Clang || AB_COMPILER_IS_AppleClang
/* Type-safe ARRAY_SIZE macro */
# define AB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + \
        sizeof(__typeof__(int[1 - 2 * \
                __builtin_types_compatible_p(__typeof__(arr), \
                    __typeof__(&arr[0]))])) * 0)
#else
# define AB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif
#define AB_DEGREES(X) ((X) * 180.0 / PI)
#define AB_RADIANS(X) ((X) * PI / 180.0)
#define AB_MAX(X, Y) (X > Y ? X : Y)
#define AB_MIN(X, Y) (X > Y ? Y : X)
#define AB_CLAMP(X, Y, Z) MIN(MAX(X, Y), Z)
#define AB_PI 3.14159265359

/************************************************
 * Logging and Assertion Macros
 ************************************************/
#define AB_LOG_INFO(...)     SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_INFO, __VA_ARGS__)
#define AB_LOG_DEBUG(...)    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_DEBUG, __VA_ARGS__)
#define AB_LOG_WARN(...)     SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_WARN, __VA_ARGS__)
#define AB_LOG_ERROR(...)    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_ERROR, __VA_ARGS__)
#define AB_LOG_CRITICAL(...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_CRITICAL, __VA_ARGS__)

#define AB_ASSERT(X)          SDL_assert(X)
#define AB_ASSERT_RELEASE(X)  SDL_assert_release(X)
#define AB_ASSERT_PARANOID(X) SDL_assert_paranoid(X)

#define AB_LOG_SETLEVEL(X) SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, X)

#define AB_STATIC_ASSERT(cond) (sizeof(char[1 - 2 * !(cond)]) * 0)

#endif /* AMBER_UTIL_COMMON_H */
