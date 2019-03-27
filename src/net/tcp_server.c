#include <tcp_server.h>
#include "dbg.h"

int io_server_connect(tcp_server_t * context, int port,
	sockstat_t (*on_received)(void *, buffer_t *), void * arg) {
	int ret;
	if (context) {
		d_trace_d(context->tag, "run server");
		do {
			d_trace_d(context->tag, "open socket");
			if ((ret = socket_get(context->socket_dsc, context->port)) < 0) {
				d_trace_e(context->tag, "open socket failed");
				break;
			}

			d_trace_d(context->tag, "bind socket");
			if ((ret = socket_bind(context->socket_dsc)) != 0) {
				d_trace_e(context->tag, "bind socket failed");
				break;
			}

			d_trace_d(context->tag, "listen socket");
			if ((ret = socket_listen(context->socket_dsc)) != 0) {
				d_trace_e(context->tag, "listen socket failed");
				break;
			}

			d_trace_d(context->tag, "run server socket success");
			context->port = port;
			context->on_received = on_received;
			context->arg = arg;
			return ret;
		} while (0);
	}
	return ret;
}

int io_server_run(tcp_server_t * context) {
	int ret = -1;
	sockstat_t sock_ret;
	if (context) {
		d_trace_d(context->tag, "run server socket listener");

		if (!context->on_received || !context->buffer) {
			d_trace_e(context->tag, "run server socket listener failed");
			return ret;
		}
		ret = 0;
		while (1) {
			if (!context->accepted) {
				d_trace_d(context->tag, "accept socket");
				if ((ret = socket_accept(context->socket_dsc)) < 0) {
					d_trace_e(context->tag, "accept socket failed");
					break;
				}
				context->accepted = true;
			}

			d_trace_i(context->tag, "send socket");
			if ((ret = socket_send(context->socket_dsc,
				context->buffer->data, context->buffer->bytes)) < 0) {
				d_trace_e(context->tag, "send socket failed");
				break;
			}

			d_trace_i(context->tag, "recv socket");
			if ((ret = socket_recv(context->socket_dsc,
				context->buffer->data, context->buffer->size)) < 0) {
				d_trace_e(context->tag, "recv socket failed");
				break;
			}

			d_trace_d(context->tag, "server callback");
			sock_ret = context->on_received(context->arg, context->buffer);
			if (sock_ret != CS_CONTINUE) {
				if (socket_shutdown(context->socket_dsc, 2) != 0) {
					d_trace_e(context->tag, "socket shutdown failed");
					break;
				}
				if (sock_ret == CS_FAILED) {
					ret = -1;
					d_trace_e(context->tag, "server callback failed");
					break;
				}
				if (sock_ret == CS_CLOSE) {
					ret = 0;
					d_trace_d(context->tag, "server close requested");
					break;
				}
				if (sock_ret == CS_BREAK) {
					context->accepted = false;
				}
			}
		}

		if (ret < 0) {
			d_trace_e(context->tag, "server socket listener exited with error: %d", ret);
		}

		d_trace_d(context->tag, "close server socket");
		ret = socket_close(context->socket_dsc);
		if (ret < 0) {
			d_trace_e(context->tag, "close server socket error: %d", ret);
		}
	}
	return ret;
}

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <memory.h>
//
//#define SRV_PORT 1234
//#define BUF_SIZE 64
//
//#define TXT_QUEST "Who are you?\n"
//
//main () {
//	int s, s_new;
//	int from_len;
//	char buf[BUF_SIZE];
//	struct sockaddr_in sin, from_sin;
//
//	s = socket (AF_INET, SOCK_STREAM, 0);
//	memset ((char *)&sin, '\0', sizeof(sin));
//	sin.sin_family = AF_INET;
//	sin.sin_addr.s_addr = INADDR_ANY;
//	sin.sin_port = SRV_PORT;
//	bind (s, (struct sockaddr *)&sin, sizeof(sin));
//	listen (s, 3);
//
//	while (1) {
//		from_len = sizeof(from_sin);
//		s_new = accept (s, &from_sin, &from_len);
//		write (s_new, TXT_QUEST, sizeof(TXT_QUEST));
//		from_len = read (s_new, buf, BUF_SIZE);
//		write (1, buf, from_len);
//		shutdown (s_new, 0);
//		close (s_new);
//	};
//}
