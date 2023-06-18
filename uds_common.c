/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "socket_common.h"
#include "fd.h"
#include "uds_common.h"

ssize_t uds_send_data(int sockfd, const void *buf, size_t len, int flags)
{
    ssize_t bytes;

    bytes = send(sockfd, buf, len, flags);
    if (bytes != len) {
        fprintf(stderr, "send error\n");
    }

    fprintf(stdout, "Sent buffer from fd %d, length=%d bytes\n", sockfd, bytes);

    return bytes;
}

ssize_t uds_recv_data(int sockfd, char *buf, ssize_t len, int flags)
{
    ssize_t bytes;
    ssize_t pos;
    int count;
    fd_set readfds, writefds;
    struct timeval timeout;

    pos = 0;
    do {
        fprintf(stdout, "Receiving buffer from fd %d\n", sockfd);

        bytes = recv(sockfd, buf+pos, len-pos, flags);
        if (bytes < 0) {
            fprintf(stderr, "recv error\n");
            break;
        } else if (bytes == 0) {
            /* No data left, jump out */
            break;
        } else {
            pos += bytes;
            if (pos >= len) {
                /* The buffer is full, jump out */
                break;
            }
        }

        fprintf(stdout, "Received buffer from fd %d, length=%d bytes\n", sockfd, bytes);

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(sockfd, &readfds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 10*1000;

        count = select(sockfd + 1, &readfds, &writefds, (fd_set *)0, &timeout);
        if (count <= 0)  {
            break;
        }
    } while (count > 0);

    return pos;
}

void *uds_handler_routine(void *arg)
{
    struct socket_connect *socket_connected = (struct socket_connect *)arg;
    ssize_t bytes, req_len, resp_len;
    uint8_t buf[UDS_BUF_SIZE];
    char *msg = "Hello World";
    socket_command_t *command = NULL;
    socket_command_t *command_resp = NULL;

    if (socket_connected == NULL) {
        fprintf(stderr, "Error: invalid argument of thread routine\n");
        pthread_exit(0);
    }

    //while (1)
    {
        req_len = uds_recv_data(socket_connected->client_fd, (char *)buf, sizeof(buf), 0);
        if (req_len <= 0) {
            close(socket_connected->client_fd);
            socket_connected->inuse = 0;
            //break;
        }

        command = (socket_command_t *) buf;

        fprintf(stdout, "Received buffer command: [%d]\n", command->command);

        command_resp = socket_command_msg_build(command->command);
        //resp_len = sizeof(socket_command_t) + command_resp->data_len;
        resp_len = command_resp->data_len;

        bytes = uds_send_data(socket_connected->client_fd,
                                command_resp->data,
                                command_resp->data_len,
                                MSG_NOSIGNAL);
        if (bytes != resp_len) {
            fprintf(stderr, "Error: send response error\n");
            close(socket_connected->client_fd);
            socket_connected->inuse = 0;
            //break;
        }

        socket_command_msg_free(command->command, command_resp);
    }

    pthread_exit(0);
}