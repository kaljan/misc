#ifndef BUF_H
#define BUF_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "node.h"

typedef struct _buf_t {
	uint8_t * data;
	size_t size;
	size_t bytes;
} buffer_t;

buffer_t * create_buffer(size_t size);
void destroy_buffer(void ** object);
ssize_t write_buffer(buffer_t * buf, uint8_t * data, size_t size);
ssize_t read_buffer(buffer_t * buf, uint8_t * data, size_t size);

s_node_t * create_double_buffer(void *object);
void destroy_double_buffer(s_node_t ** item);
void swap_buffer(s_node_t ** item);

#endif // BUF_H
