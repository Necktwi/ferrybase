/**
 */

#include "logger.h"
#include "mystdlib.h"
#include <cstdarg>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef __CYGWIN__
#include <pthread.h>
#else
#include <sys/syscall.h>
#endif
#include <stdlib.h>

/**
 * Log level count
 */
#define FFLT_COUNT 10
static const char * const log_type_names[] = {
	"ERR",
	"WRN",
	"NTC",
	"DBG",
	"INF",
	"PSR",
	"HDR",
	"EXT",
	"CLN",
	"LNC",
};

/*int _ff_log(_ff_log_type t, const char* format, ...) {
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
			sprintf(buf2, "[%s %s]: ", (const char*) getuTime().c_str(), log_type_names[n]);
			break;
		}

	fprintf(stderr, "%s%s\n", buf2, buf);
	fflush(stderr);
	return 0;
};*/

int _ff_log(const char* func, _ff_log_type allowedType, _ff_log_type t,
		unsigned int allowedLevel, unsigned int l, const char* format, ...) {
	char buf[1000];
	unsigned int nnl = 1 << 31;
	bool no_new_line = (l & nnl) == nnl;
	l &= ~nnl;
	va_list ap;

	if (!(allowedType & t) || !(allowedLevel & l))
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
			sprintf(buf2, "[%s %s %s]: ", (const char*) getTime().c_str(), log_type_names[n], func);
			break;
		}
	if (no_new_line) {
		fprintf(stderr, "%s%s", buf2, buf);
	} else {
		fprintf(stderr, "%s%s\n", buf2, buf);
	}
	fflush(stderr);
	return 0;
};

int _ff_log(_ff_log_type allowedType,
		_ff_log_type t,
		unsigned int allowedLevel,
		unsigned int l,
		const char* func,
		const char* file_name,
		int line_no,
		const char* format, ...) {
	char buf[1000];
	unsigned int nnl = 1 << 31;
	bool no_new_line = (l & nnl) == nnl;
	l &= ~nnl;
	va_list ap;

	if (!(allowedType & t) || !(allowedLevel & l))
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
			sprintf(buf2, "[%s %s %05d %s:%s:%d]: ", (const char*) getuTime().c_str(), log_type_names[n],
#ifdef __CYGWIN__
					pthread_self(),
#else
					syscall(SYS_gettid),
#endif
					func, file_name, line_no);
			break;
		}

	if (no_new_line) {
		fprintf(stderr, "%s%s", buf2, buf);
	} else {
		fprintf(stderr, "%s%s\n", buf2, buf);
	}
	fflush(stderr);
	return 0;
};

int _ff_log_contnu(_ff_log_type allowedType, _ff_log_type t,
		unsigned int allowedLevel,
		unsigned int l,
		const char* format,
		...) {
	char buf[1000];
	unsigned int nnl = 1 << 31;
	bool no_new_line = (l & nnl) == nnl;
	l &= ~nnl;
	va_list ap;

	if (!(allowedType & t) || !(allowedLevel & l))
		return 0;

	va_start(ap, format);
	vsnprintf(buf, sizeof (buf), format, ap);
	buf[sizeof (buf) - 1] = '\0';
	va_end(ap);

	if (no_new_line) {
		fprintf(stderr, "%s", buf);
	} else {
		fprintf(stderr, "%s\n", buf);
	}
	fflush(stderr);
	return 0;
};

bool _ffl_level(_ff_log_type allowedType, _ff_log_type t,
		unsigned int allowedLevel,
		unsigned int l) {
	if ((allowedType & t == t)&&(allowedLevel & l == l)) {
		return true;
	}
	return false;
};