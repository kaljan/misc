#ifndef IOFS_SERIAL_PORT_H
#define IOFS_SERIAL_PORT_H

#include <termios.h>
#include <sys/ioctl.h>
#include "file.h"
#include "lpthread.h"

typedef enum  {
	BAUD_50,
	BAUD_75,
	BAUD_110,
	BAUD_134,
	BAUD_150,
	BAUD_200,
	BAUD_300,
	BAUD_600,
	BAUD_1200,
	BAUD_1800,
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_19200,
	BAUD_38400,
	BAUD_57600,
	BAUD_115200,
	BAUD_230400,
	BAUD_460800,
	BAUD_500000,
	BAUD_576000,
	BAUD_921600,
	BAUD_1000000,
	BAUD_1152000,
	BAUD_1500000,
	BAUD_2000000,
	BAUD_2500000,
	BAUD_3000000,
	BAUD_3500000,
	BAUD_4000000
} tty_baud_rate_t;

typedef enum {
	STOP_1 = 0,
	STOP_2 = 1
} tty_stop_bits_t;

typedef enum {
	PAR_NONE = 0,
	PAR_ODD = 1,
	PAR_EVEN = 2
} tty_parity_t;

typedef struct _tty_settings_t {
	char * name;
	tty_baud_rate_t baudrate;
	tty_stop_bits_t stop_bits;
	tty_parity_t parity;
} tty_settings_t;

typedef struct _tty_port_t {
	_Bool in_progress;
	tty_settings_t * settings;
	struct termios options;
	file_t * file;
	lpthread_t * thread;
	pthread_mutex_t * mutex;
	_Bool (*listener)(void *,void *, size_t);
	void (*on_failed)(void *, int, const char *);
	void * arg;
} tty_port_t;

#define TTY_BUFFER_SIZE	4096

int bytes_available(tty_port_t * context);
ssize_t tty_port_write(tty_port_t * context, const void * data, size_t size);
ssize_t tty_port_read(tty_port_t * context, void * data, size_t size);
void serial_port_destroy(tty_port_t ** context);
tty_port_t * serial_port_create(const char * name,
	tty_settings_t * settings, _Bool nolock);
int tty_port_set_listener(tty_port_t * context, _Bool (*listener)(void *, void *, size_t),
	void (*on_failed)(void *, int, const char *), void * arg);
int tty_port_run(tty_port_t * context);
void tty_port_abort(tty_port_t * context);
int serial_port_open(tty_port_t * context);
void serial_port_close(tty_port_t * context);

#endif // IOFS_SERIAL_PORT_H
