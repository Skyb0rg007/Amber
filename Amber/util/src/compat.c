#include <Amber/util/compat.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#if defined(AB_NEED_STRDUP)
char *AB_strdup(const char *str)
{
    size_t len = strlen(str) + 1;
    void *copy = malloc(len);
    if (copy == NULL)
        return NULL;

    memcpy(copy, str, len);
    return copy;
}
#endif

#if defined(AB_NEED_VASPRINTF)
int AB_vasprintf(char **strp, const char *fmt, va_list args)
{
    int size = 0;
    va_list temp;

    va_copy(temp, args); /* requires c99 */

    size = vsnprintf(NULL, 0, fmt, temp);

    va_end(temp);

    if (size < 0)
        return -1;

    *strp = malloc(size + 1);
    if (*strp == NULL)
        return -1;

    size = vsnprintf(*strp, size, fmt, args);
    return size;
}
#endif

#if defined(AB_NEED_ASPRINTF)
int AB_asprintf(char **strp, const char *fmt, ...)
{
    va_list va;
    int ret;

    va_start(va, fmt);
    ret = AB_vasprintf(strp, fmt, va);

    va_end(va);
    return ret;
}
#endif

#if defined(AB_NEED_STRTOK_R)
char *AB_strtok_r(char *str, const char *delim, char **saveptr)
{
    char *end;

    /* Load string from saveptr if not the first time */
    if (str == NULL)
        str = *saveptr;

    /* Reached the end of the inpupt string */
    if (str[0] == '\0') {
        *saveptr = str;
        return NULL;
    }

    /* skip all characters in 'delim' */
    str += strspn(str, delim);
    /* check for end again */
    if (str[0] == '\0') {
        *saveptr = str;
        return NULL;
    }

    /* skip all characters NOT in 'delim' */
    end = str + strcspn(str, delim);

    /* check for end of string */
    if (*end == '\0') {
        *saveptr = end;
        return str;
    }

    /* Write in a null and update saveptr */
    *end = '\0';
    *saveptr = end + 1;
    return str;
}
#endif
