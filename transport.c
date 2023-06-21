/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#include <arpa/inet.h>

#include <string.h>

#include "transport.h"
#include "transport_private.h"
#include "uds.h"

struct transport_create_s {
    enum transport_type type;

    struct transport * (*create)(void);
};

extern struct transport *uds_transport_create(void);

static struct transport_create_s transport_create_tb[] =
{
    {TRANS_UDS, uds_transport_create},
};

int transport_close(struct transport *t, int sockfd)
{
	return t->close(t, sockfd);
}

int transport_open_server(struct transport *t, char *uds_path, struct fdarray *fda)
{
    return t->open_server(t, uds_path, fda);
}

int transport_open_client(struct transport *t, char *uds_path, int timeout, int *fd)
{
    return t->open_client(t, uds_path, timeout, fd);
}

int transport_connect(struct transport *t, char *uds_path, int timeout)
{
    return t->connect(t, uds_path, timeout);
}

int transport_accept(struct transport *t, struct socket_connect *connect[], struct fdarray *fda)
{
    return t->accept(t, connect, fda);
}

int transport_send(struct transport *t, int fd, void *data, int len)
{
    return t->send(t, fd, 0, data, len);
}

int transport_recv(struct transport *t, int fd, char *resp, int resp_len)
{
    return t->recv(t, fd, resp, resp_len);
}

struct transport *transport_create(enum transport_type type)
{
	struct transport *t = NULL;
	
    if (transport_create_tb[type].create)
    {
        t = transport_create_tb[type].create();
    }

	if (t) {
		t->type = type;
	}

	return t;
}

void transport_destroy(struct transport *t)
{
	t->release(t);
}
