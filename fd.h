/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_FD_H
#define HAVE_FD_H

#define N_TIMER_FDS 8

enum {
	FD_EVENT,
	FD_GENERAL,

	N_POLLFD,
};

struct fdarray {
	int fd[N_POLLFD];
};

#endif /* HAVE_FD_H */
