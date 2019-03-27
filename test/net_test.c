
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <tcp_client.h>
#include <tcp_server.h>
#include "dbg.h"
#include "rnd.h"
#include "zlib.h"

#define IO_SERVER_BUFFER_SIZE 64
#define RND_STR_LENGHT	32

const char server_socket_tag[] = "server_socket\0";
const char server_name[] = "asus-x556u\0";

socket_dsc_t server_socket = {
	.s_connected = false,
	.s_client = false,
	.s_des = 0,
	.s_acpt_des = 0,
	.s_domain = 0,
	.s_type = 0,
	.s_protocol = 0,
	.s_addrlen = 0,
	.s_queue_len = 0,
	.s_flags = 0,
	.s_error_no = 0,
	.s_name = server_name,
	.tag = server_socket_tag,
	.s_hostent = NULL,
};

uint8_t io_server_data[IO_SERVER_BUFFER_SIZE];

buffer_t io_server_buffer = {
	.data = io_server_data,
	.size = IO_SERVER_BUFFER_SIZE,
	.bytes = 0,
};

tcp_server_t io_server = {
	.port = 1234,
	.socket_dsc = &server_socket,
	.buffer = &io_server_buffer,
	.tag = server_socket_tag,
	.on_received = NULL,
	.arg = NULL,
};

const char client_socket_tag[] = "client_socket\0";
const char client_name[] = "test_clnt\0";

socket_dsc_t client_socket = {
	.s_connected = false,
	.s_client = true,
	.s_des = 0,
	.s_acpt_des = 0,
	.s_domain = 0,
	.s_type = 0,
	.s_protocol = 0,
	.s_addrlen = 0,
	.s_queue_len = 0,
	.s_flags = 0,
	.s_error_no = 0,
	.s_name = server_name,
	.tag = client_socket_tag,
	.s_hostent = NULL,
};

uint8_t io_client_data[IO_SERVER_BUFFER_SIZE];

buffer_t io_client_buffer = {
	.data = io_client_data,
	.size = IO_SERVER_BUFFER_SIZE,
	.bytes = 0,
};

tcp_client_t io_client = {
	.port = 1234,
	.socket_dsc = &client_socket,
	.buffer = &io_client_buffer,
	.tag = client_socket_tag,
	.on_received = NULL,
	.arg = NULL,
};

char rnd_str_buf[RND_STR_LENGHT + 1];

int prepare_paket(buffer_t * buffer) {
	int ret = -1;
	if (buffer && buffer->data) {
		uLong crc;
		d_trace_d(server_socket_tag, "prepare new paket");
		frandstr(rnd_str_buf, RND_STR_LENGHT);
		d_trace_d(server_socket_tag, "data: \"%s\"", rnd_str_buf);
		crc = crc32(0, (const Bytef *)rnd_str_buf, RND_STR_LENGHT);
		d_trace_d(server_socket_tag, "crc : 0x%08X", (uint32_t)crc);
		memmove(buffer->data, (void *)rnd_str_buf, RND_STR_LENGHT);
		memmove(buffer->data + RND_STR_LENGHT, (void *)(&crc), sizeof(crc));
		buffer->bytes = RND_STR_LENGHT + 5;
		ret = 0;
	}
	return ret;
}

int parse_paket(buffer_t * buffer) {
	int ret = -1;
	if (buffer && buffer->data) {
		char str[RND_STR_LENGHT + 1];
		uLong crc, c_crc;
		d_trace_d(server_socket_tag, "parse new paket");
		memmove((void *)str, buffer->data, RND_STR_LENGHT);
		str[RND_STR_LENGHT] = 0;
		d_trace_d(server_socket_tag, "data: \"%s\"", str);
		memmove((void *)&crc, buffer->data + RND_STR_LENGHT, sizeof(crc));
		d_trace_d(server_socket_tag, "crc : 0x%08X", (uint32_t)crc);
		c_crc = crc32(0, (const Bytef *)str, RND_STR_LENGHT);
		d_trace_d(server_socket_tag, "crc : 0x%08X", (uint32_t)c_crc);
		ret = memcmp((void *)&crc, (void *)&c_crc, sizeof(crc));
		d_trace_d(server_socket_tag, "ret : %d", ret);
	}
	return ret;
}

int check_exit(buffer_t * buffer) {
	int ret = -1;
	if (buffer && buffer->data) {
		char str[5];
		memmove((void *)str, buffer->data, 4);
		str[4] = 0;
		ret = strcmp(str, "exit\0");
	}
	return ret;
}

int check_cmd(buffer_t * buffer, const char *cmd) {
	int ret = -1;
	if (buffer && buffer->data && cmd && strlen(cmd) > 0) {
		char str[5];
		memmove((void *)str, buffer->data, 4);
		str[4] = 0;
		ret = strcmp(str, cmd);
	}
	return ret;
}

sockstat_t io_server_on_received(void * arg, buffer_t * buffer) {
	if (buffer) {
		d_trace_d(server_socket_tag, "request from client received");
		do {
			if (check_cmd(buffer, "exit") == 0) {
				d_trace_d(server_socket_tag, "exit received");
				return CS_CLOSE;
			}

			if (check_cmd(buffer, "quit") == 0) {
				d_trace_d(server_socket_tag, "quit received");
				return CS_BREAK;
			}

			d_trace_d(server_socket_tag, "parse data");
			parse_paket(buffer);
			d_trace_d(server_socket_tag, "prepare answer");
			prepare_paket(buffer);
			return CS_CONTINUE;
		} while (0);
	}
	return CS_FAILED;
}

int run_server_test(void) {
	int ret = io_server_connect(&io_server, 1234, io_server_on_received, NULL);
	if (ret == 0) {
		initrand();
		prepare_paket(io_server.buffer);
		ret = io_server_run(&io_server);
	}
	return ret;
}

sockstat_t io_client_on_received(void * arg, buffer_t * buffer) {
	if (buffer) {
		if (buffer->data) {
			d_trace_t(client_socket_tag, "%s", (const char *)buffer->data);
		}
		if (arg) {
			int v = *((int *)arg);
			if (v == 0) {
				sprintf((char *)buffer->data, "quit");
				buffer->bytes = 5;
				return CS_BREAK;
			}
			v--;
			*((int *)arg) = v;
			d_trace_d(client_socket_tag, "parse data");
			parse_paket(buffer);
			d_trace_d(client_socket_tag, "prepare answer");
			prepare_paket(buffer);
			return CS_CONTINUE;
		}
	}
	return CS_FAILED;
}

int run_client_test(void) {
	int index;
	char * c_addr;
	int ret = io_client_init(&io_client, 1234, io_client_on_received, &index);
	if (ret == 0) {
		initrand();
		c_addr = straddr(&io_client.socket_dsc->s_clnt_addr,
			(socklen_t)io_client.socket_dsc->s_addrlen);
		d_trace_i(client_socket_tag, "ADDR: %s", c_addr);
		c_addr = straddr(&io_client.socket_dsc->s_srv_addr,
			(socklen_t)io_client.socket_dsc->s_addrlen);
		d_trace_i(client_socket_tag, "ADDR: %s", c_addr);
		index = randint(4, 20);
		ret = io_client_request(&io_client);
	}
	return ret;
}

int net_test(int argc, char ** argv) {
	if (argc < 2) {
		return run_server_test();
	}
	return run_client_test();
}

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <errno.h>
//#include <stdio.h>
//
//const char name[] = "localhost\0";
//
//int main(int argc, char ** argv) {
//
//	struct hostent * host = gethostbyname(*(argv + 1));
//	if (host) {
//		char ** ptr = host->h_aliases;
//		printf(
//			"h_name    : \"%s\"\n"
//			"h_addrtype: %d\n"
//			"h_lenght  : %d\n"
//			, host->h_name
//			, host->h_addrtype
//			, host->h_length
//		);
//
//		for (int i = 0; i < host->h_length; i++) {
//			if (i < host->h_length - 1) {
//				printf("%d.", (int)((uint8_t)host->h_addr[i]));
//				continue;
//			}
//			printf("%d", (int)((uint8_t)host->h_addr[i]));
//		}
//		printf("\n");
//		while (*ptr) {
//			printf("h_alias: \"%s\"\n", *ptr);
//		}
//		ptr = host->h_addr_list;
////		while (*ptr) {
////			printf("h_addr : \"%s\"\n", *ptr);
////		}
//	}
//
//	return 0;
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

//	struct timeval tv;
//	struct tm * t;
//	char str[256];
//	gettimeofday(&tv, NULL);
//	t = localtime((time_t *)(&tv.tv_sec));
//	strftime(str, 255, "%H:%M:%S", t);
//	printf("%s.%04d\n", str, tv.tv_usec / 1000);
//	LOGDT("123123");
//	d_trace_v("V", "verbose %d", argc);
//	d_trace_i("I", "info    %d", argc);
//	d_trace_t("T", "thread  %d", argc);
//	d_trace_w("W", "warning %d", argc);
//	d_trace_e("e", "error   %d", argc);
