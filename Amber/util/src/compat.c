#include <Amber/util/compat.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#if defined(AB_NEED_STRDUP)
char *AB_strdup(const char *str)
{
    size_t len;
    void *copy;

    len = strlen(str) + 1;
    copy = malloc(len);
    if (copy == NULL)
        return NULL;

    memcpy(copy, str, len);
    return copy;
}
#endif

#if defined(AB_NEED_VASPRINTF)
int AB_vasprintf(char **strp, const char *fmt, va_list ap)
{
    size_t size;

    {
        va_list tmp;

        AB_VA_COPY(tmp, ap); /* requires C99 or compiler support */
        size = (size_t)vsnprintf(NULL, 0, fmt, tmp);
        va_end(tmp);
    }

    if (size < 0)
        return -1;

    *strp = malloc(size + 1);
    if (*strp == NULL)
        return -1;

    return vsnprintf(*strp, size, fmt, ap);
}
#endif

#if defined(AB_NEED_ASPRINTF)
int AB_asprintf(char **strp, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = AB_vasprintf(strp, fmt, ap);

    va_end(ap);
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

#if defined(AB_NEED_GETLINE)
#include <stdlib.h>
long AB_getline(char **lineptr, size_t *n, FILE *stream)
{
    char *ptr, *eptr;

    if (lineptr == NULL || n == NULL || stream == NULL)
        return -1;

    if (*lineptr == NULL || *n == 0) {
        *n = BUFSIZ;
        *lineptr = malloc(*n);
        if (*lineptr == NULL)
            return -1;
    }

    ptr = *lineptr;
    eptr = *lineptr + *n;
    for (;;) {
        int c = fgetc(stream);
        if (c == EOF) {
            if (feof(stream)) {
                long diff = ptr - *lineptr;
                if (diff != 0) {
                    *ptr = '\0';
                    return diff;
                }
            }
            return -1;
        }

        *ptr++ = (char)c;
        if (c == '\n') {
            *ptr = '\0';
            return ptr - *lineptr;
        }
        if (ptr + 2 >= eptr) {
            char *tmp;
            size_t new_size = *n * 2;
            long diff = ptr - *lineptr;

            tmp = realloc(*lineptr, new_size);
            if (tmp == NULL)
                return -1;

            *lineptr = tmp;
            *n = new_size;
            eptr = *lineptr + *n;
            ptr = *lineptr + diff;
        }
    }
}
#endif

#if defined(AB_NEED_VSNPRINTF)
int AB_vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    char *buf = NULL;
    int res = AB_VASPRINTF(&buf, fmt, ap);

    if (buf == NULL)
        return -1;
    if (res < 0) {
        free(buf);
        return -1;
    }

    res = strlen(buf);
    if (n > 0) {
        if ((long)n > res) {
            memcpy(out, buf, res+1);
        } else {
            memcpy(out, buf, res-1);
            out[n-1] = '\0';
        }
    }

    free(buf);
    return res;
}
#endif

#if defined(AB_NEED_SNPRINTF)
int AB_snprintf(char *out, size_t n, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = AB_VSNPRINTF(out, n, fmt, ap);

    va_end(ap);
    return ret;
}
#endif
