#include <tcp_client.h>
#include "dbg.h"

int io_client_init(tcp_client_t * context, int port,
	sockstat_t (*on_received)(void *, buffer_t *), void * arg) {
	int ret = -1;
	if (context) {
		d_trace_d(context->tag, "io_client init");
		do {
			d_trace_d(context->tag, "open socket");
			if ((ret = socket_get(context->socket_dsc, port)) < 0) {
				d_trace_e(context->tag, "open socket failed");
				break;
			}

			d_trace_d(context->tag, "get host by name: \"%s\"", context->socket_dsc->s_name);
			if ((ret = socket_get_host_by_name(context->socket_dsc)) != 0) {
				d_trace_e(context->tag, "get host socket failed");
				break;
			}
			d_trace_d(context->tag, "connect socket");
			if ((ret = socket_connect(context->socket_dsc)) != 0) {
				d_trace_e(context->tag, "connect socket failed");
				break;
			}
			context->socket_dsc->s_client = true;
			context->on_received = on_received;
			context->arg = arg;
		} while (0);
	}
	return ret;
}

int io_client_request(tcp_client_t * context) {
	int ret = -1;
	sockstat_t sock_ret;
	if (context && context->buffer && context->buffer->data) {
		while (1) {
			d_trace_i(context->tag, "recv socket");
			if ((ret = socket_recv(context->socket_dsc,
				context->buffer->data, context->buffer->size)) < 0) {
				d_trace_e(context->tag, "recv socket failed");
				break;
			}
			d_trace_d(context->tag, "data received");
			if (context->on_received) {
				d_trace_d(context->tag, "run callback");
				sock_ret = context->on_received(context->arg, context->buffer);
			}
			d_trace_i(context->tag, "send socket");
			if (socket_send(context->socket_dsc,
				context->buffer->data, context->buffer->bytes) < 0) {
				d_trace_e(context->tag, "send socket failed");
				break;
			}
			if (sock_ret != CS_CONTINUE) {
				if (sock_ret == CS_FAILED) {
					d_trace_e(context->tag, "callback failed");
					break;
				}
				d_trace_d(context->tag, "exit requested");
				break;
			}
		}
		d_trace_d(context->tag, "exit socket; %d", ret);
		ret = socket_close(context->socket_dsc);
		d_trace_d(context->tag, "socket closed; %d", ret);
	}
	return ret;
}

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <memory.h>
//
//#define SRV_HOST "delta"
//#define SRV_PORT 1234
//#define CLNT_PORT 1235
//#define BUF_SIZE 64
//#define TXT_ANSW "I am your client\n"
//
//main () {
//	int s;
//	int from_len;
//	char buf[BUF_SIZE];
//	struct hostent *hp;
//	struct sockaddr_in clnt_sin, srv_sin;
//
//	s = socket (AF_INET, SOCK_STREAM, 0);
//	memset ((char *)&clnt_sin, '\0', sizeof(clnt_sin));
//	clnt_sin.sin_family = AF_INET;
//	clnt_sin.sin_addr.s_addr = INADDR_ANY;
//	clnt_sin.sin_port = CLNT_PORT;
//	bind (s, (struct sockaddr *)&clnt_sin, sizeof(clnt_sin));
//	memset ((char *)&srv_sin, '\0', sizeof(srv_sin));
//	hp = gethostbyname (SRV_HOST);
//	srv_sin.sin_family = AF_INET;
//	memcpy ((char *)&srv_sin.sin_addr,hp->h_addr,hp->h_length);
//	srv_sin.sin_port = SRV_PORT;
//	connect (s, &srv_sin, sizeof(srv_sin));
//	from_len = recv (s, buf, BUF_SIZE, 0);
//	write (1, buf, from_len);
//	send (s, TXT_ANSW, sizeof(TXT_ANSW), 0);
//	close (s);
//	exit (0);
//
//}
