/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#include "uds.h"
#include "container.h"
#include "transport_private.h"
#include "socket_common.h"
#include "uds_common.h"
#include "print.h"

#define UDS_FILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)

struct uds {
	struct transport t;
	struct address address;
};

static int uds_open_server(struct transport *t, char *uds_path, struct fdarray *fda)
{
	struct uds *uds = container_of(t, struct uds, t);
	struct sockaddr_un sa;
	int fd, err;

	unlink(uds_path);

    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, uds_path);

	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd < 0) {
		fprintf(stderr, "uds: failed to create socket: %m\n");
		return -1;
	}

	err = bind(fd, (struct sockaddr *) &sa, sizeof(sa));
	if (err < 0) {
		fprintf(stderr, "uds: bind failed: %m\n");
		close(fd);
		return -1;
	}

    err = listen(fd, SOCKET_MAX_BACKLOG);
    if (err != 0) {
        fprintf(stderr, "listen error\n");
        close(fd);
        return -1;
    }

	uds->address.sun = sa;
	uds->address.len = sizeof(sa);

	chmod(uds_path, UDS_FILEMODE);
	fda->fd[FD_EVENT] = -1;
	fda->fd[FD_GENERAL] = fd;

    fprintf(stdout, "%s:%d Opened transport UDS %s successfully!\n", __func__, __LINE__, uds_path);

	return 0;
}

static int uds_open_client(struct transport *t, char *uds_path, int timeout, int *fd)
{
	socket_client_t *client = container_of(t, socket_client_t, t);
	struct sockaddr_un sa;
	int ret_fd, err;

	ret_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (ret_fd < 0) {
		fprintf(stderr, "uds: failed to create socket: %m\n");
		return -1;
	}

    client->sockfd = ret_fd;
    *fd = ret_fd;
    printf("open client %p fd = %d\n", client, client->sockfd);

    fprintf(stdout, "%s:%d Opened transport UDS fd %d successfully!\n", __func__, __LINE__, ret_fd);

	return 0;
}

static int uds_connect(struct transport *t, char *uds_path, int timeout)
{
	socket_client_t *client = container_of(t, socket_client_t, t);
    struct sockaddr_un sa;
	int err;

    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, uds_path);

    fprintf(stdout, "%s:%d Connecting transport UDS fd %d %s!\n", __func__, __LINE__, client->sockfd, uds_path);
    do {
        err = connect(client->sockfd, (struct sockaddr *)&sa, sizeof(sa));
        if (err == 0) {
            break;
        } else {
            sleep(1);
        }
    } while (timeout-- > 0);

    if (err != 0) {
        fprintf(stderr, "connect error\n");
        close(client->sockfd);
        return -1;
    }

    fprintf(stdout, "%s:%d Connected transport UDS %s successfully!\n", __func__, __LINE__, uds_path);

	return 0;
}

static int uds_accept(struct transport *t, struct socket_connect *connect[], struct fdarray *fda)
{
    struct socket_connect *socket_connected = NULL;
    socket_server_t *server = container_of(t, socket_server_t, t);
    int client_fd = 0;
    int index = 0;

    fprintf(stdout, "%s:%d Accepting transport UDS!\n", __func__, __LINE__);

    client_fd = accept(fda->fd[FD_GENERAL], NULL, NULL);
    if (client_fd < 0) {
        fprintf(stderr, "accept error\n");
        return -1;
    }

    fprintf(stdout, "%s:%d Accepted client %d!\n", __func__, __LINE__, client_fd);

    for (index = 0; index < SOCKET_MAX_CLIENT; index++) {
        if (server->conn[index].inuse == 0) {
            break;
        }
    }

    if (index >= SOCKET_MAX_CLIENT) {
        fprintf(stderr, "Error: too many connections\n");
        close(client_fd);
        return -1;
    }

    socket_connected = &server->conn[index];
    socket_connected->client_fd = client_fd;
    socket_connected->inuse = 1;

    if (pthread_create(&socket_connected->thread_id, NULL,
                        uds_handler_routine, socket_connected) != 0) {
        fprintf(stderr, "pthread_create error\n");
        close(client_fd);
        socket_connected->inuse = 0;
        return -1;
    }

	return 0;
}

static void uds_release(struct transport *t)
{
	struct uds *uds = container_of(t, struct uds, t);
    fprintf(stdout, "%s:%d Releasing %p\n", __func__, __LINE__, uds);
	free(uds);

    fprintf(stdout, "%s:%d Released transport UDS successfully!\n", __func__, __LINE__);
}

static int uds_send(struct transport *t, int fd, int peer, void *buf, int buflen)
{
	return uds_send_data(fd, buf, buflen, MSG_NOSIGNAL);
}

static int uds_recv(struct transport *t, int fd, char *resp, int resp_len)
{
	return uds_recv_data(fd, resp, resp_len, MSG_NOSIGNAL);
}

struct transport *uds_transport_create(void)
{
	struct uds *uds;
	uds = calloc(1, sizeof(struct uds));

	if (!uds)
		return NULL;

    uds->t.open_server = uds_open_server;
    uds->t.open_client = uds_open_client;
    uds->t.accept      = uds_accept;
    uds->t.connect     = uds_connect;
    uds->t.release     = uds_release;
    uds->t.send        = uds_send;
    uds->t.recv        = uds_recv;

    fprintf(stdout, "%s:%d Created transport UDS %p successfully!\n", __func__, __LINE__, uds);

	return &uds->t;
}
