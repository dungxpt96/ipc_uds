/******************************************************************************
*
* FILENAME:
*     server.c
*
* DESCRIPTION:
*     The example of server using Unix domain socket.
*
******************************************************************************/

#include <unistd.h>
#include <signal.h>

#include "error.h"
#include "uds.h"
#include "transport.h"
#include "socket_common.h"

volatile sig_atomic_t loop_flag = 1;

struct socket_type {
    enum transport_type type;
    char *path;
};

static struct socket_type socket_type_tb[] =
{
    {TRANS_UDS, UDS_FILE_PATH},
};

void handler_sigint(int sig)
{
    loop_flag = 0;
}

void install_sig_handler()
{
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_handler = handler_sigint;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
}

int main(void)
{
    enum transport_type type = TRANS_UDS;
    socket_server_t *server = NULL;

    install_sig_handler();

    server = socket_server_allocate();
    if (!server)
        return ERROR_ALLOCATION;

    server->t = transport_create(type);
    if (!server->t)
        return ERROR_ALLOCATION;

    transport_open_server(server->t, socket_type_tb[type].path, &server->fda);

    while (loop_flag) {
        transport_accept(server->t, &server->conn, &server->fda);
    }

    transport_destroy(server->t);
    socket_server_deallocate(server);

    return ERROR_NOERR;
}