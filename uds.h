/******************************************************************************
*
* FILENAME:
*     
*
* DESCRIPTION:
*     
*
******************************************************************************/

#ifndef HAVE_UDS_H
#define HAVE_UDS_H

#include "transport.h"

#define UDS_FILE_PATH       "/tmp/uds_server_client_123456"

struct transport *uds_transport_create(void);

#endif /* HAVE_UDS_H */
