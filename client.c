/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

#include "error.h"
#include "uds.h"
#include "transport.h"
#include "socket_common.h"
#include "smc_msg.h"

struct socket_type {
    enum transport_type type;
    char *path;
    int timeout;
};

static struct socket_type socket_type_tb[] =
{
    {TRANS_UDS, UDS_FILE_PATH, 10},
};

int main(void)
{
    enum transport_type type = TRANS_UDS;
    socket_client_t *client = NULL;
    socket_command_t *command = NULL;
    socket_command_t *command_resp = NULL;
    char resp[1024];

    client = socket_client_allocate();
    if (!client)
        return ERROR_ALLOCATION;

    client->t = transport_create(type);
    if (!client->t)
        return ERROR_ALLOCATION;

    transport_open_client(client->t, socket_type_tb[type].path,
                socket_type_tb[type].timeout, &client->sockfd);

    printf("open client %p fd = %d\n", client, client->sockfd);

    transport_connect(client->t, socket_type_tb[type].path, socket_type_tb[type].timeout);

    command = socket_command_build(SOCKET_COMMAND_VERSION_GET);

    transport_send(client->t, client->sockfd,
                        command,
                        sizeof(socket_command_t));
    
    transport_recv(client->t, client->sockfd,
                        resp,
                        sizeof(resp));

    socket_command_print(SOCKET_COMMAND_VERSION_GET, resp);

    socket_command_free(command);

    //transport_destroy(client->t);
    socket_client_deallocate(client);

    return ERROR_NOERR;
}