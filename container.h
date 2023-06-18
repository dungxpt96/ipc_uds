/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_CONTAIN_H
#define HAVE_CONTAIN_H

#include <stddef.h>

/*
 * This macro borrowed from the Linux kernel.
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type, member) );	\
})

#endif /* HAVE_CONTAIN_H */
