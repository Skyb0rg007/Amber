#include <Amber/util/common.h>

void AB_QUICK_LOG(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    AB_LOGV(AB_LOG_CATEGORY_GENERAL, AB_LOG_PRIORITY_INFO, fmt, ap);
    va_end(ap);
}

void AB_assert_expr_func(int cond, const char *str, const char *file, int line) {
    if (!cond) {
        AB_LOG(AB_LOG_CATEGORY_GENERAL, AB_LOG_PRIORITY_CRITICAL,
                "[%s:%d] Assertion \"%s\" failed!", file, line, str);
        abort();
    }
}

const char *AB_strerror(AB_errno_t err)
{
    switch (err)
    {
    case AB_OK:
        return "No error";
    case AB_ENOMEM:
        return "Out of memory";
    default:
        return "Unknown error code";
    }
}
