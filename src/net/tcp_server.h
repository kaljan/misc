#ifndef NET_IO_SERVER_H
#define NET_IO_SERVER_H

#include <netsock.h>
#include "buffer.h"

typedef struct _tcp_server_t {
	_Bool accepted;
	int port;
	socket_dsc_t * socket_dsc;
	buffer_t * buffer;
	const char * tag;
	sockstat_t (*on_received)(void *, buffer_t *);
	void * arg;
} tcp_server_t;

int io_server_connect(tcp_server_t * context, int port,
	sockstat_t (*on_received)(void *, buffer_t *), void * arg);
int io_server_run(tcp_server_t * context);

#endif // NET_IO_SERVER_H
