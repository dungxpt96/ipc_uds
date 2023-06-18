/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_PRINT_H
#define HAVE_PRINT_H

#include <syslog.h>

#define PRINT_LEVEL_MIN LOG_EMERG
#define PRINT_LEVEL_MAX LOG_DEBUG

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
void print(int level, char const *format, ...);

void print_set_progname(const char *name);
void print_set_tag(const char *tag);
void print_set_syslog(int value);
void print_set_level(int level);
void print_set_verbose(int value);

#define pr_emerg(x...)   print(LOG_EMERG, x)
#define pr_alert(x...)   print(LOG_ALERT, x)
#define pr_crit(x...)    print(LOG_CRIT, x)
#define pr_err(x...)     print(LOG_ERR, x)
#define pr_warning(x...) print(LOG_WARNING, x)
#define pr_notice(x...)  print(LOG_NOTICE, x)
#define pr_info(x...)    print(LOG_INFO, x)
#define pr_debug(x...)   print(LOG_DEBUG, x)

#define PRINT_RL(l, i, x...) \
	do { \
		static time_t last = -i; \
		if (!rate_limited(i, &last)) \
			print(l, x); \
	} while (0);

/* Rate limited versions */
#define pl_emerg(i, x...)   PRINT_RL(LOG_EMERG, i, x)
#define pl_alert(i, x...)   PRINT_RL(LOG_ALERT, i, x)
#define pl_crit(i, x...)    PRINT_RL(LOG_CRIT, i, x)
#define pl_err(i, x...)     PRINT_RL(LOG_ERR, i, x)
#define pl_warning(i, x...) PRINT_RL(LOG_WARNING, i, x)
#define pl_notice(i, x...)  PRINT_RL(LOG_NOTICE, i, x)
#define pl_info(i, x...)    PRINT_RL(LOG_INFO, i, x)
#define pl_debug(i, x...)   PRINT_RL(LOG_DEBUG, i, x)

#endif
