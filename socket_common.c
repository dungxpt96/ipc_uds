#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "socket_common.h"
#include "fd.h"

socket_client_t *
socket_client_allocate(void)
{
    socket_client_t *socket = calloc(1, sizeof(socket_client_t));
    return socket;
}

void
socket_client_deallocate(socket_client_t *socket)
{
    close(socket->sockfd);

    if (socket)
        free(socket);
}

socket_server_t *
socket_server_allocate(void)
{
    socket_server_t *socket = calloc(1, sizeof(socket_server_t));
    return socket;
}

void
socket_server_deallocate(socket_server_t *socket)
{
    int index = 0;
    for (index = 0; index < SOCKET_MAX_CLIENT; index++) {
        if (socket->conn[index].inuse) {
            pthread_join(socket->conn[index].thread_id, NULL);
            close(socket->conn[index].client_fd);
        }
    }

    close(socket->fda.fd[FD_GENERAL]);

    if (socket)
        free(socket);
}

socket_command_t *
socket_command_build(uint32_t command_id)
{
    socket_command_t *command = calloc (1, sizeof(socket_command_t));
    command->command = command_id;
    return command;
}

void
socket_command_free(socket_command_t *command)
{
    if (command)
        free(command);
}

socket_command_t *
socket_command_msg_build(uint32_t command_id)
{
    socket_command_t *command = calloc (1, sizeof(socket_command_t));
    command->command = SOCKET_COMMAND_SUCCESS;
    //fprintf(stdout, "%s:%d command_id=%d \n", __func__, __LINE__, command_id);
    smc_msg_build(command_id, &command->data, &command->data_len);
    socket_command_print(SOCKET_COMMAND_VERSION_GET, command->data);
    return command;
}

void
socket_command_msg_free(uint32_t command_id, socket_command_t *command)
{
    if (command)
    {
        smc_msg_free(command_id, command->data);
        free(command);
    }
}

void
socket_command_print(uint32_t command_id, void *data)
{
    smc_msg_print(command_id, data);
}