#include <netsock.h>
#include "dbg.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

char * straddr(struct sockaddr_in * addr, socklen_t len) {
	char str[255] = {0};
	inet_ntop(AF_INET, &addr->sin_addr, str, len);
	return strdup(str);
}

socket_dsc_t * socket_create(int port, _Bool client) {
	socket_dsc_t * context = (socket_dsc_t *)
		malloc(sizeof(socket_dsc_t));
	if (context) {
		context->s_type = SOCK_STREAM;
		context->s_domain = AF_INET;
		context->s_protocol = 0;
		context->s_port = port;
		context->s_connected = false;
		context->s_client = client;
		if ((context->s_des = socket(context->s_domain,
			context->s_type, context->s_protocol)) < 0) {
			free(context);
			return NULL;
		}
		context->s_addrlen = sizeof(context->s_srv_addr);
		memset((void *)(&context->s_srv_addr), 0,
			context->s_addrlen);
		context->s_acpt_des = -1;
		if (context->s_client) {
			context->s_clnt_addr.sin_port = port;
			context->s_clnt_addr.sin_family = AF_INET;
			context->s_clnt_addr.sin_addr.s_addr = INADDR_ANY;
		} else {
			context->s_srv_addr.sin_port = port;
			context->s_srv_addr.sin_family = AF_INET;
			context->s_srv_addr.sin_addr.s_addr = INADDR_ANY;
		}
	}
	return context;
}

void socket_destroy(socket_dsc_t ** context) {
	if (context && context) {
		free(*context);
		*context = NULL;
	}
}

int socket_get(socket_dsc_t * context, int port) {
	int ret = -1;
	if (context) {
		d_trace_d(context->tag, "create socket; port: %d;", port);
		context->s_type = SOCK_STREAM;
		context->s_domain = AF_INET;
		context->s_protocol = 0;
		context->s_port = port;
		context->s_connected = false;
		ret = socket(context->s_domain,
			context->s_type, context->s_protocol);
		if (ret >= 0) {
			d_trace_d(context->tag, "socket created");
			context->s_addrlen = sizeof(context->s_srv_addr);
			memset((void *)(&context->s_clnt_addr), 0,
				context->s_addrlen);
			memset((void *)(&context->s_clnt_addr), 0,
				context->s_addrlen);
			context->s_des = ret;
			context->s_acpt_des = ret;
			if (context->s_client) {
				context->s_clnt_addr.sin_port = port;
				context->s_clnt_addr.sin_family = AF_INET;
				context->s_clnt_addr.sin_addr.s_addr = INADDR_ANY;
			} else {
				context->s_srv_addr.sin_port = port;
				context->s_srv_addr.sin_family = AF_INET;
				context->s_srv_addr.sin_addr.s_addr = INADDR_ANY;
			}

			return ret;
		}
		d_trace_errno(context->tag, "create socket failed; ");
	}
	return ret;
}

int socket_bind(socket_dsc_t * context) {
	int ret = 0;
	if (context) {
		d_trace_d(context->tag, "bind socket; s_des: %d", context->s_des);
		if (context->s_client) {
			d_trace_d(context->tag, "bind as client socket");
			ret = bind(context->s_des,
				(struct sockaddr *)&context->s_clnt_addr,
				(socklen_t)context->s_addrlen);
		} else {
			d_trace_d(context->tag, "bind as server");
			ret = bind(context->s_des,
				(struct sockaddr *)&context->s_srv_addr,
				(socklen_t)context->s_addrlen);
		}
		if (ret < 0) {
			d_trace_errno(context->tag, "bind socket %d failed; ", context->s_des);
			return ret;
		}
		d_trace_d(context->tag, "bind socket %d success; ", context->s_des);
	}
	return ret;
}

int socket_connect(socket_dsc_t * context) {
	int ret = 0;
	if (context) {
		d_trace_d(context->tag, "connect socket");
		ret = connect(context->s_des,
			(struct sockaddr *)&context->s_srv_addr, context->s_addrlen);
		if (ret != 0) {
			d_trace_errno(context->tag, "connect socket %d failed; ", context->s_des);
			return ret;
		}
		context->s_connected = true;
		d_trace_d(context->tag, "connect socket success");
	}
	return ret;
}

int socket_listen(socket_dsc_t * context) {
	int ret = 0;
	if (context) {
		d_trace_d(context->tag, "listen socket");
		ret = listen(context->s_des, context->s_queue_len);
		if (ret != 0) {
			d_trace_errno(context->tag, "listen socket failed");
		}
		d_trace_d(context->tag, "listen socket success");
	}
	return ret;
}

int socket_accept(socket_dsc_t * context) {
	int ret = -1;
	d_trace_d(context->tag, "accept socket");
	if (context && ((ret = accept(context->s_des,
		(struct sockaddr *)&context->s_clnt_addr,
		(socklen_t *)&context->s_addrlen)) >= 0)) {
		context->s_acpt_des = ret;
		if (ret < 0) {
			d_trace_errno(context->tag, "accept socket failed");
			return ret;
		}
		d_trace_d(context->tag, "accept socket success");
	}
	return ret;
}

int socket_send(socket_dsc_t * context, void * buf, size_t len) {
	int ret = -1;
	if (context) {
		d_trace_d(context->tag, "socket send");
		ret = send(context->s_acpt_des, buf, len, context->s_flags);
		d_trace_d(context->tag, "socket send: %d", ret);
		if (ret < 0) {
			d_trace_errno(context->tag, "socket send failed");
			return ret;
		}
		d_trace_d(context->tag, "socket send success; %d", ret);
	}
	return ret;
}

int socket_recv(socket_dsc_t * context, void * buf, size_t len) {
	int ret = -1;
	if (context) {
		d_trace_d(context->tag, "socket recv");
		ret = recv(context->s_acpt_des, buf, len, context->s_flags);
		if (ret < 0) {
			d_trace_errno(context->tag, "socket recv failed");
			return ret;
		}
		d_trace_d(context->tag, "socket recv success; %d", ret);
	}
	return ret;
}

int socket_shutdown(socket_dsc_t * context, int mode) {
	int ret = -1;
	if (context) {
		d_trace_d(context->tag, "socket shutdown; fd: %d; acpt_fd: %d;",
			context->s_des, context->s_acpt_des);
		ret = shutdown(context->s_acpt_des, mode);
		if (ret < 0) {
			d_trace_errno(context->tag, "socket shutdown failed");
			return ret;
		}
		d_trace_d(context->tag, "socket shutdown success");
	}
	return ret;
}

int socket_close(socket_dsc_t * context) {
	int ret = -1;
	if (context) {
		d_trace_d(context->tag, "socket close");
		ret = close(context->s_des);
		if (ret < 0) {
			d_trace_errno(context->tag, "socket close failed");
			return ret;
		}
		d_trace_d(context->tag, "socket close success");
	}
	return ret;
}

int socket_get_host_by_name(socket_dsc_t * context) {
	int ret = -1;
	if (context) {
		memset((char *)&context->s_srv_addr, '\0', sizeof(context->s_srv_addr));
		d_trace_v(context->tag, "get host name");
		if ((context->s_hostent = gethostbyname(context->s_name))) {
			ret = 0;
			d_trace_v(context->tag, "get host name success");
			context->s_srv_addr.sin_family = AF_INET;
			memcpy ((char *)&context->s_srv_addr.sin_addr,
				context->s_hostent->h_addr, context->s_hostent->h_length);
			context->s_srv_addr.sin_port = context->s_port;
			return ret;
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

//	int s;
//	int from_len;
//	char buf[BUF_SIZE];
//	struct hostent *hp;
//	struct sockaddr_in clnt_sin, srv_sin;

//	memset ((char *)&srv_sin, '\0', sizeof(srv_sin));
//	hp = gethostbyname (SRV_HOST);
//	srv_sin.sin_family = AF_INET;
//	memcpy ((char *)&srv_sin.sin_addr,hp->h_addr,hp->h_length);
//	srv_sin.sin_port = SRV_PORT;
