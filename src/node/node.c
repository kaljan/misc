#include "node.h"

s_node_t * s_node_create(void * object, void * next) {
	if (object) {
		s_node_t * node = (s_node_t *)malloc(sizeof(s_node_t));
		if (node) {
			node->object = object;
			node->next = next;
		}
		return node;
	}
	return NULL;
}

void s_node_destroy(s_node_t ** node) {
	if (node && *node) {
		free((void *)*node);
		*node = NULL;
	}
}

d_node_t * d_node_create(void * object, void * prev, void * next) {
	if (object) {
		d_node_t * node = (d_node_t *)malloc(sizeof(d_node_t));
		if (node) {
			node->object = object;
			node->next = next;
			node->prev = prev;
		}
		return node;
	}
	return NULL;
}

void d_node_destroy(d_node_t ** node) {
	if (node && *node) {
		free((void *)*node);
		*node = NULL;
	}
}
