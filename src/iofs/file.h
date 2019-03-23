#ifndef IOFS_FILE_H
#define IOFS_FILE_H

#include <fcntl.h>
#include <errno.h>
#include "lpthread.h"
#include "buffer.h"

typedef struct _file_t {
	char * f_name;
	char * f_path;
	char * f_pathname;
	int f_des;
	int f_flags;
	struct stat f_stat;
	uint8_t * f_md;
	int error_no;
} file_t;

void file_destroy_instance(file_t ** instance);
file_t * file_create_instance(const char * pathname);
int file_open(file_t * instance);
void file_close(file_t * instance);
ssize_t file_read(file_t * instance, uint8_t * data, size_t size);
ssize_t file_write(file_t * instance, uint8_t * data, size_t size);

#endif // IOFS_FILE_H
