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
#include <stdint.h>
#include <stdio.h>

#include "smc_msg.h"
#include "socket_common.h"

struct smc_msg_handler
{
    uint32_t command;
    void (*build)(void **data, int *len);
    void (*free)(void *data);
    void (*print)(void *data);
};

static struct smc_msg_handler smc_msg_handler_tb[] = 
{
    {SOCKET_COMMAND_SUCCESS,      NULL,                   NULL,                  NULL},
    {SOCKET_COMMAND_FAIL,         NULL,                   NULL,                  NULL},
    {SOCKET_COMMAND_VERSION_GET,  smc_msg_version_build,  smc_msg_version_free,  smc_msg_version_print},
};

void
smc_msg_version_build(void **data, int *len)
{
    struct smc_msg_version *msg = calloc(1, sizeof (struct smc_msg_version));

    strcpy(msg->version, "SMC VERSION 1.0.0");

    *len = sizeof (struct smc_msg_version);
    *data = msg;
}

void
smc_msg_version_free(void *data)
{
    struct smc_msg_version *msg = (struct smc_msg_version *)data;
    free(msg);
}

void
smc_msg_version_print(void *data)
{
    struct smc_msg_version *msg = (struct smc_msg_version *)data;
    
    fprintf(stdout, "=================================\n");
    fprintf(stdout, "Version SMC: %s\n", msg ? msg->version : "NULL");
    fprintf(stdout, "=================================\n");
}

void
smc_msg_build(uint32_t command_id, void **data, int *len)
{
    //fprintf(stdout, "%s:%d command_id=%d \n", __func__, __LINE__, command_id);
    if (smc_msg_handler_tb[command_id].build)
    {
        smc_msg_handler_tb[command_id].build(data, len);
    }
}

void
smc_msg_free(uint32_t command_id, void *data)
{
    //fprintf(stdout, "%s:%d command_id=%d \n", __func__, __LINE__, command_id);
    if (smc_msg_handler_tb[command_id].free)
    {
        smc_msg_handler_tb[command_id].free(data);
    }
}

void
smc_msg_print(uint32_t command_id, void *data)
{
    //fprintf(stdout, "%s:%d command_id=%d \n", __func__, __LINE__, command_id);
    if (smc_msg_handler_tb[command_id].print)
    {
        smc_msg_handler_tb[command_id].print(data);
    }
}