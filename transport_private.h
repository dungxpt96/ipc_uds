/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_TRANSPORT_PRIVATE_H
#define HAVE_TRANSPORT_PRIVATE_H

#include <time.h>

#include "transport.h"
#include "address.h"
#include "socket_common.h"

struct transport {
	enum transport_type type;

    int (*open_server)(struct transport *t, char *filename, struct fdarray *fda);

    int (*open_client)(struct transport *t, char *filename, int timeout, int *fd);
    
    int (*connect)(struct transport *t, char *filename, int timeout);

    int (*accept)(struct transport *t, struct socket_connect **connect, struct fdarray *fda);

    int (*close)(struct transport *t, struct fdarray *fda);

	int (*recv)(struct transport *t, int fd, char *resp, int resp_len);

	int (*send)(struct transport *t, int fd, int peer, void *buf, int buflen);

    void (*release)(struct transport *t);
};

#endif /* HAVE_TRANSPORT_PRIVATE_H */
