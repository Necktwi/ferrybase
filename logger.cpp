/**
 */

#include "logger.h"
#include "mystdlib.h"
#include <cstdarg>
#include <stdio.h>

/**
 * Log level count
 */
#define FFLT_COUNT 10
static const char * const log_type_names[] = {
    "ERR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
    "PARSER",
    "HEADER",
    "EXTENSION",
    "CLIENT",
    "LATENCY",
};

int _ff_log(_ff_log_type t, const char* format, ...) {
    char buf[300];
    va_list ap;

    if (!(ff_log_type & t))
        return 0;

    va_start(ap, format);
    vsnprintf(buf, sizeof (buf), format, ap);
    buf[sizeof (buf) - 1] = '\0';
    va_end(ap);

    char buf2[300];
    unsigned long long now;
    int n;

    buf2[0] = '\0';
    for (n = 0; n < FFLT_COUNT; n++)
        if (t == (1 << n)) {
            //now = time_in_microseconds() / 100;
            sprintf(buf2, "[%s %s]: ", (const char*) getTime().c_str(), log_type_names[n]);
            break;
        }

    fprintf(stderr, "%s%s\n", buf2, buf);
    return 0;
};

int _ff_log(_ff_log_type t, int l, const char* format, ...) {
    char buf[300];
    va_list ap;

    if (!(ff_log_type & t) || !(ff_log_level & l))
        return 0;

    va_start(ap, format);
    vsnprintf(buf, sizeof (buf), format, ap);
    buf[sizeof (buf) - 1] = '\0';
    va_end(ap);

    char buf2[300];
    unsigned long long now;
    int n;

    buf2[0] = '\0';
    for (n = 0; n < FFLT_COUNT; n++)
        if (t == (1 << n)) {
            //now = time_in_microseconds() / 100;
            sprintf(buf2, "[%s %s]: ", (const char*) getTime().c_str(), log_type_names[n]);
            break;
        }

    fprintf(stderr, "%s%s\n", buf2, buf);
    return 0;
};

int _ff_log(_ff_log_type t, const char* func, const char* file_name, int line_no, const char* format, ...) {
    char buf[300];
    va_list ap;

    if (!(ff_log_level & t))
        return 0;

    va_start(ap, format);
    vsnprintf(buf, sizeof (buf), format, ap);
    buf[sizeof (buf) - 1] = '\0';
    va_end(ap);

    char buf2[300];
    unsigned long long now;
    int n;

    buf2[0] = '\0';
    for (n = 0; n < FFLT_COUNT; n++)
        if (t == (1 << n)) {
            //now = time_in_microseconds() / 100;
            sprintf(buf2, "[%s %s %s:%s:%d]: ", (const char*) getTime().c_str(), log_type_names[n], func, file_name, line_no);
            break;
        }

    fprintf(stderr, "%s%s\n", buf2, buf);
    return 0;
};

