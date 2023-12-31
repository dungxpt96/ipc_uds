/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "print.h"

static int verbose = 0;
static int print_level = LOG_INFO;
static int use_syslog = 1;
static const char *progname;
static const char *message_tag;

void print_set_progname(const char *name)
{
	progname = name;
}

void print_set_tag(const char *tag)
{
	message_tag = tag;
}

void print_set_syslog(int value)
{
	use_syslog = value ? 1 : 0;
}

void print_set_level(int level)
{
	print_level = level;
}

void print_set_verbose(int value)
{
	verbose = value ? 1 : 0;
}

void print(int level, char const *format, ...)
{
	struct timespec ts;
	va_list ap;
	char buf[1024];
	FILE *f;

	if (level > print_level)
		return;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);

	if (verbose) {
		f = level >= LOG_NOTICE ? stdout : stderr;
		fprintf(f, "%s[%lld.%03ld]: %s%s%s\n",
			progname ? progname : "",
			(long long)ts.tv_sec, ts.tv_nsec / 1000000,
			message_tag ? message_tag : "", message_tag ? " " : "",
			buf);
		fflush(f);
	}
	if (use_syslog) {
		syslog(level, "[%lld.%03ld] %s%s%s",
		       (long long)ts.tv_sec, ts.tv_nsec / 1000000,
		       message_tag ? message_tag : "", message_tag ? " " : "",
		       buf);
	}
}
