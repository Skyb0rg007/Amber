#ifndef AMBER_UTIL_COMMON_H
#define AMBER_UTIL_COMMON_H

#include <Amber/util/compat.h>
#include <SDL_assert.h>
#include <SDL_log.h>
#include <assert.h>

/************************************************
 * Convenience Macros
 ************************************************/
#define AB_ARRAY_SIZE(arr) (sizeof (arr) / sizeof ((arr)[0]))
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

#ifdef static_assert
    #define AB_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
    #define AB_STATIC_ASSERT(cond, msg) typedef char static_assert_##__LINE__[(cond)?1:-1]
#endif

#endif /* AMBER_UTIL_COMMON_H */
