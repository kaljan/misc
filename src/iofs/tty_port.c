#include "tty_port.h"
#include "str.h"

void tty_lock_mutex(tty_port_t * context) {
	if (context && context->mutex) {
		pthread_mutex_lock(context->mutex);
	}
}

void tty_unlock_mutex(tty_port_t * context) {
	if (context && context->mutex) {
		pthread_mutex_unlock(context->mutex);
	}
}

int set_baudrate(tty_port_t * context) {
	int ret = -1;
	if (context && context->settings) {
		speed_t speed;
		switch (context->settings->baudrate) {
		case BAUD_50: speed = B50; break;
		case BAUD_75: speed = B75; break;
		case BAUD_110: speed = B110; break;
		case BAUD_134: speed = B134; break;
		case BAUD_150: speed = B150; break;
		case BAUD_200: speed = B200; break;
		case BAUD_300: speed = B300; break;
		case BAUD_600: speed = B600; break;
		case BAUD_1200: speed = B1200; break;
		case BAUD_1800: speed = B1800; break;
		case BAUD_2400: speed = B2400; break;
		case BAUD_4800: speed = B4800; break;
		case BAUD_9600: speed = B9600; break;
		case BAUD_19200: speed = B19200; break;
		case BAUD_38400: speed = B38400; break;
		case BAUD_57600: speed = B57600; break;
		case BAUD_115200: speed = B115200; break;
		case BAUD_230400: speed = B230400; break;
		case BAUD_460800: speed = B460800; break;
		case BAUD_500000: speed = B500000; break;
		case BAUD_576000: speed = B576000; break;
		case BAUD_921600: speed = B921600; break;
		case BAUD_1000000: speed = B1000000; break;
		case BAUD_1152000: speed = B1152000; break;
		case BAUD_1500000: speed = B1500000; break;
		case BAUD_2000000: speed = B2000000; break;
		case BAUD_2500000: speed = B2500000; break;
		case BAUD_3000000: speed = B3000000; break;
		case BAUD_3500000: speed = B3500000; break;
		case BAUD_4000000: speed = B4000000; break;
		default: speed = B0; break;
		}
		ret = cfsetspeed(&context->options, speed);
	}
	return ret;
}

void set_stopbits(tty_port_t * context) {
	if (context && context->settings) {
		if (context->settings->stop_bits == STOP_1) {
			context->options.c_cflag &= ~CSTOPB;
			return;
		}
		context->options.c_cflag |= CSTOPB;
	}
}

void set_parity(tty_port_t * context) {
	if (context && context->settings) {
		context->options.c_iflag &= ~(INPCK | IGNPAR | PARMRK | ISTRIP);

		switch (context->settings->parity) {
		default:
		case PAR_NONE:
			context->options.c_cflag &= ~(PARENB | PARODD);
			break;

		case PAR_ODD:
			context->options.c_cflag |= PARENB | PARODD;
			context->options.c_iflag |= INPCK | ISTRIP;

			break;

		case PAR_EVEN:
			context->options.c_cflag |= PARENB;
			context->options.c_cflag &= ~PARODD;
			context->options.c_iflag |= INPCK | ISTRIP;
			break;
		}
	}
}

int bytes_available(tty_port_t * context) {
	int bytes = -1;
	if (context && context->file) {
		if (ioctl(context->file->f_des, FIONREAD, &bytes) != 0) {
			return -1;
		}
	}
	return bytes;
}

_Bool tty_ready_read(void * arg) {
	if (arg) {
		tty_port_t * context = (tty_port_t *)arg;
		int bytes_avail;
		if (context->settings) {
			char buffer[TTY_BUFFER_SIZE];
			ssize_t bytes_read = -1;
			bytes_avail = bytes_available(context);
			if (bytes_avail < 0) {
				if (context->on_failed) {
					char * error_str = __fstr(
						"Failed to get available bytes in tty \"%s\""
							, context->settings->name);

					context->on_failed(context->arg, bytes_avail, (const char *)error_str);
					free(error_str);
				}
				return false;
			}
			if (bytes_avail == 0) {
				return true;
			}
			if (bytes_avail > TTY_BUFFER_SIZE) {
				bytes_avail = TTY_BUFFER_SIZE;
			}
			bytes_read = tty_port_read(context, (void *)buffer, (size_t)bytes_avail);
			if (bytes_read < 0) {
				if (context->on_failed) {
					char * error_str =
						__fstr("Failed to read tty \"%s\". ERROR(%d)"
							, context->settings->name
							, context->file->error_no
							, strerror(context->file->error_no));

					context->on_failed(context->arg, bytes_read, (const char *)error_str);
					free(error_str);
				}
				return false;
			}
			if (context->listener) {
				return context->listener(context->arg, buffer, (size_t)bytes_read);
			}
		}
	}
	return false;
}

int tty_port_update_settings(tty_port_t * context, _Bool nolock) {
	int ret = -1;
	if (context && !((ret = file_open(context->file)) < 0)) {
		do {
			if ((ret = tcgetattr(context->file->f_des,
				&context->options)) != 0) {
				break;
			}

			context->options.c_cflag |= CLOCAL | CREAD;
			context->options.c_cflag &= ~PARENB;
			context->options.c_cflag &= ~CSTOPB;
			context->options.c_cflag &= ~CSIZE;
			context->options.c_cflag |= CS8;
			context->options.c_cflag &= ~CRTSCTS;
			context->options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
			context->options.c_iflag &= ~(INPCK | ISTRIP | ICRNL);
			context->options.c_iflag &= ~(IXON | IXOFF | IXANY);
			context->options.c_oflag &= ~(OPOST | ONLCR);

			if (set_baudrate(context) < 0) {
				break;
			}

			set_stopbits(context);
			set_parity(context);

			if ((ret = tcsetattr(context->file->f_des,
				TCSANOW, &context->options)) != 0) {
				break;
			}
			if (nolock) {
				ret = fcntl(context->file->f_des, F_SETFL, FNDELAY);
			}
		} while (0);
		file_close(context);
	}
	return ret;
}

ssize_t tty_port_write(tty_port_t * context, const void * data, size_t size) {
	ssize_t bytes_written = -1;
	if (context) {
		tty_lock_mutex(context);
		bytes_written = file_write(context->file, data, size);
		tty_unlock_mutex(context);
	}
	return bytes_written;
}

ssize_t tty_port_read(tty_port_t * context, void * data, size_t size) {
	ssize_t bytes_written = -1;
	if (context) {
		tty_lock_mutex(context);
		bytes_written = file_read(context->file, data, size);
		tty_unlock_mutex(context);
	}
	return bytes_written;
}

void serial_port_destroy(tty_port_t ** context) {
	if (context && *context) {
		file_destroy_instance(&(*context)->file);
		lpthread_destroy(&(*context)->thread);
		lpthread_destroy_mutex(&(*context)->mutex);
		free((void *)*context);
		*context = NULL;
	}
}

tty_port_t * serial_port_create(const char * name,
	tty_settings_t * settings, _Bool nolock) {
	tty_port_t * context = NULL;
	if (settings && (context = (tty_port_t *)malloc(sizeof(tty_port_t)))) {
		memset(context, 0, sizeof(tty_port_t));
		context->settings = settings;
		if ((context->file = file_create_instance(context->settings->name)) &&
			(tty_port_update_settings(context, nolock) == 0)) {
			return context;
		}
		serial_port_destroy(&context);
	}
	return context;
}

int tty_port_set_listener(tty_port_t * context, _Bool (*listener)(void *, void *, size_t),
	_Bool (*on_failed)(void *, int, const char *), void * arg) {
	int ret = -1;
	if (context) {
		char * thread_tag = __fstr("TREAD(%s)", context->settings->name);
		if (!(context->mutex = lpthread_create_mutex(PTHREAD_MUTEX_DEFAULT))) {
			return ret;
		}
		if ((context->thread = lpthread_get_context(
			NULL, tty_ready_read, NULL, thread_tag, (void *)context))) {
			lpthread_destroy_mutex(&context->mutex);
			return ret;
		}
		ret = 0;
		context->listener = listener;
		context->on_failed = on_failed;
		context->arg = arg;
	}
	return ret;
}

int tty_port_run(tty_port_t * context) {
	int ret = -1;
	if (context) {
		ret = lpthread_run(context->thread);
	}
	return ret;
}

void tty_port_abort(tty_port_t * context) {
	if (context) {
		lpthread_abort(context->thread);
	}
}

int serial_port_open(tty_port_t * context) {
	int ret = -1;
	if (context) {
		ret = file_open(context->file);
	}
	return ret;
}

void serial_port_close(tty_port_t * context) {
	if (context) {
		file_close(context->file);
	}
}
