#ifndef AMBER_UTIL_COMMON_H
#define AMBER_UTIL_COMMON_H

#include <SDL_assert.h>
#include <assert.h>

/************************************************
 * Convenience Macros
 ************************************************/
#define ARRAY_SIZE(arr) (sizeof (arr) / sizeof ((arr)[0]))
#define PI 3.14159265359
#define DEGREES(X) ((X) * 180.0 / PI)
#define RADIANS(X) ((X) * PI / 180.0)

/************************************************
 * Logging and Assertion Macros
 ************************************************/
#define LOG_INFO(...)     SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_INFO, __VA_ARGS__)
#define LOG_DEBUG(...)    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_DEBUG, __VA_ARGS__)
#define LOG_WARN(...)     SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_WARN, __VA_ARGS__)
#define LOG_ERROR(...)    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_ERROR, __VA_ARGS__)
#define LOG_CRITICAL(...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, \
        SDL_LOG_PRIORITY_CRITICAL, __VA_ARGS__)

#define ASSERT(X)          SDL_assert(X)
#define ASSERT_RELEASE(X)  SDL_assert_release(X)
#define ASSERT_PARANOID(X) SDL_assert_paranoid(X)

#define LOG_SETLEVEL(X) SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, X)

#ifdef static_assert
    #define STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
    #define STATIC_ASSERT(cond, msg) typedef char static_assert_##__LINE__[(cond)?1:-1]
#endif

#endif /* AMBER_UTIL_COMMON_H */
