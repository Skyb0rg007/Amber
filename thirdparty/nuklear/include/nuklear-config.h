#ifndef NUKLEAR_CONFIG_H
#define NUKLEAR_CONFIG_H


/* Options for both header and implementation */
#include <stdarg.h>
#include <assert.h>

#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS
/* #define NK_INCLUDE_COMMAND_USERDATA */
/* #define NK_UINT_DRAW_INDEX */
#define NK_ASSERT    assert


/* Options for implementation */
#ifdef NK_IMPLEMENTATION
#include <string.h>
#include <math.h>
#define NK_MEMSET    memset
#define NK_MEMCPY    memcpy
#define NK_SQRT      sqrt
#define NK_SIN       sin
#define NK_COS       cos
#define NK_STRTOD    my_strtod
#define NK_DTOA      my_dtoa
#define NK_VSNPRINTF vsnprintf

static double my_strtod(const char *s, const char **endptr);
static char *my_dtoa(char *s, double n);

#endif /* NK_IMPLEMENTATION */


/* Include the real header */
#include <nuklear.h>


/* Definitions for the implementation */
#ifdef NK_IMPLEMENTATION
static double my_strtod(const char *s, const char **endptr)
{
    return strtod(s, (char **)endptr);
}

static char *my_dtoa(char *s, double n)
{
    snprintf(s, NK_MAX_NUMBER_BUFFER, "%f", n);
    return s;
}
#endif /* NK_IMPLEMENTATION */

#endif /* NUKLEAR_CONFIG_H */
