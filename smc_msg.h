/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_SMC_MSG_H
#define HAVE_SMC_MSG_H

#include <unistd.h>

struct smc_msg_version
{
    char version[128];
};

void
smc_msg_version_build(void **data, int *len);

void
smc_msg_version_free(void *data);

void
smc_msg_version_print(void *data);

void
smc_msg_build(uint32_t command_id, void **data, int *len);

void
smc_msg_free(uint32_t command_id, void *data);

void
smc_msg_print(uint32_t command_id, void *data);

#endif /* HAVE_SMC_MSG_H */
