#ifndef NET_IO_CLIENT_H
#define NET_IO_CLIENT_H

#include "netsock.h"
#include "buffer.h"

typedef struct _tcp_client_t {
	int port;
	socket_dsc_t * socket_dsc;
	buffer_t * buffer;
	const char * tag;
	struct hostent *hp;
	sockstat_t (*on_received)(void *, buffer_t *);
	void * arg;
} tcp_client_t;

int io_client_init(tcp_client_t * context, int port,
	sockstat_t (*on_received)(void *, buffer_t *), void * arg);
int io_client_request(tcp_client_t * context);
#endif // NET_IO_CLIENT_H
