/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_SOCKET_COMMON_H
#define HAVE_SOCKET_COMMON_H

#include "transport.h"
#include "fd.h"

#define SOCKET_MAX_BACKLOG     10
#define SOCKET_MAX_CLIENT      10

/* Make a structure 1-byte aligned */
#define BYTE_ALIGNED            __attribute__((packed))

typedef struct socket_command {
    union {
#define SOCKET_COMMAND_VERSION_GET      2
        uint32_t command;       /* Request type */
#define SOCKET_COMMAND_SUCCESS          0
#define SOCKET_COMMAND_FAIL             1
        uint32_t status;        /* Status code of response */
    };

    void *data;
    uint32_t data_len;          /* The data length of packet */

    //uint16_t checksum;          /* The checksum of the packet */
}socket_command_t;

struct socket_connect {
    int inuse;                  /* 1: the connection structure is in-use; 0: free */
    int client_fd;              /* Socket fd of the connection */
    pthread_t thread_id;        /* The thread id of request handler */
    struct socket_server *serv; /* The pointer of uds_server who own the connection */
};

typedef struct uds_client {
    int sockfd;         /* Socket fd of the client */
    struct transport *t;
} socket_client_t;

typedef struct socket_server {
    int sockfd;

    struct transport *t;
    struct fdarray fda;

    struct socket_connect conn[SOCKET_MAX_CLIENT];
} socket_server_t;

socket_client_t *
socket_client_allocate(void);

void
socket_client_deallocate(socket_client_t *socket);

socket_server_t *
socket_server_allocate(void);

void
socket_server_deallocate(socket_server_t *socket);

socket_command_t *
socket_command_build(uint32_t command_id);

void
socket_command_free(socket_command_t *command);

socket_command_t *
socket_command_msg_build(uint32_t command_id);

void
socket_command_msg_free(uint32_t command_id, socket_command_t *command);

void
socket_command_print(uint32_t command_id, void *data);

#endif /* HAVE_SOCKET_COMMON_H */
