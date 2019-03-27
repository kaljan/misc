#ifndef SOCK_WRAPPER_H
#define SOCK_WRAPPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define SOCKET_RBUF_SIZE	4096

typedef enum {
	CS_CLOSE = 2,
	CS_BREAK = 1,
	CS_CONTINUE = 0,
	CS_FAILED = -1,
} sockstat_t;

/**
 * @arg s_des дескриптор сокета
 *
 * @arg s_domain задает используемый для взаимодействия набор
 *      протоколов (вид коммуникационной области), для стека протоколов
 *      TCP/IP он должен иметь символьное значение AF_INET (определено в
 *      sys/socket.h).
 *
 * @arg s_type задает режим взаимодействия:
 *      • SOCK_STREAM - с установлением соединения;
 *      • SOCK_DGRAM - без установления соединения.
 *
 * @arg s_protocol Аргумент protocolзадает конкретный протокол
 *      транспортного уровня (из нескольких возможных в стеке протоколов).
 *      Если этот аргумент задан равным 0, то будет использован протокол
 *      "по умолчанию" (TCP для SOCK_STREAM и UDP для SOCK_DGRAM при
 *      использовании комплекта протоколов TCP/IP).
 *
 * @arg s_in_addr Структура sockaddr_in используется несколькими
 *      системными вызовами и функциями socket-интерфейса и определена
 *      в include-файле in.h.
 *
 *      • Поле sin_addr содержит адрес (номер) узла сети.
 *      • Поле sin_port содержит номер порта на узле сети.
 *
 * @arg s_addrlen задает размер (в байтах) структуры данных,
 *      указываемой аргументом addr.
 *
 * @arg s_queue_len Аргумент n определяет максимальную длину очереди
 *      входящих запросов на установление связи. Если какой-либо клиент
 *      выдаст запрос на установление связи при полной очереди, то этот
 *      запрос будет отвергнут.
 */
typedef struct _socket_dsc_t {
	_Bool s_connected;
	_Bool s_client;
	int s_des;
	int s_acpt_des;
	int s_port;
	int s_domain;
	int s_type;
	int s_protocol;
	int s_addrlen;
	int s_queue_len;
	int s_flags;
	int s_error_no;
	const char * s_name;
	const char * tag;
	struct hostent *s_hostent;
	struct sockaddr_in s_srv_addr;
	struct sockaddr_in s_clnt_addr;
} socket_dsc_t;

char * straddr(struct sockaddr_in * addr, socklen_t len);
int socket_get(socket_dsc_t * context, int port);
int socket_bind(socket_dsc_t * context);
int socket_connect(socket_dsc_t * context);
int socket_listen(socket_dsc_t * context);
int socket_accept(socket_dsc_t * context);
int socket_send(socket_dsc_t * context, void * buf, size_t len);
int socket_recv(socket_dsc_t * context, void * buf, size_t len);
int socket_shutdown(socket_dsc_t * context, int mode);
int socket_close(socket_dsc_t * context);
int socket_get_host_by_name(socket_dsc_t * context);

#endif // SOCK_WRAPPER_H
