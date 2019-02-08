#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct _s_node_t {
	void * object;
	void * next;
} s_node_t;

typedef struct _d_node_t {
	void * object;
	void * prev;
	void * next;
} d_node_t;

s_node_t * s_node_create(void * object, void * next);
void s_node_destroy(s_node_t ** node);

d_node_t * d_node_create(void * object, void * prev, void * next);
void d_node_destroy(d_node_t ** node);

#endif // NODE_H
