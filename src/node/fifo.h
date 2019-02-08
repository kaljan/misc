#ifndef FIFO_H
#define FIFO_H

#include "node.h"
#include <stdbool.h>

typedef struct _fifo_t {
	int max;
	int items;
	_Bool empty;
	_Bool full;
	s_node_t * in;
	s_node_t * out;
} fifo_t;

fifo_t * create_fifo(int max);
void destroy_fifo(fifo_t ** fifo, void (*destroy)(void **));
_Bool fifo_write(fifo_t * fifo, void * object);
void * fifo_read(fifo_t * fifo);
void flush_fifo(fifo_t * fifo, void (*destroy)(void **));

#endif // FIFO_H
