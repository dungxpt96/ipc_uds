/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_TRANSPORT_H
#define HAVE_TRANSPORT_H

#include "fd.h"
#include "address.h"
#include "socket_common.h"

/* Values from networkProtocol enumeration 7.4.1 Table 3 */
enum transport_type {
	/* 0 is Reserved in spec. Use it for UDS */
	TRANS_UDS = 0,
	TRANS_UDP_IPV4 = 1,
	TRANS_UDP_IPV6,
	TRANS_IEEE_802_3,
	TRANS_DEVICENET,
	TRANS_CONTROLNET,
	TRANS_PROFINET,
};

struct transport;

int transport_close(struct transport *t, int sockfd);

int transport_open_server(struct transport *t, char *uds_path, struct fdarray *fda);

int transport_open_client(struct transport *t, char *uds_path, int timeout, int *fd);

int transport_connect(struct transport *t, char *uds_path, int timeout);

int transport_accept(struct transport *t, struct socket_connect *connect[], struct fdarray *fda);

int transport_send(struct transport *t, int fd, void *data, int len);

int transport_recv(struct transport *t, int fd, char *resp, int resp_len);

struct transport *transport_create(enum transport_type type);

void transport_destroy(struct transport *t);

#endif /* HAVE_TRANSPORT_H */
