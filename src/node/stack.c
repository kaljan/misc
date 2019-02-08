#include "stack.h"

void stack_push(s_node_t ** tos, void * object) {
	if (tos) {
		*tos = s_node_create(object, *tos);
	}
}

void * stack_pop(s_node_t ** tos) {
	if (tos && *tos) {
		void * obj = (*tos)->object;
		s_node_t * t = *tos;
		*tos = (*tos)->next;
		s_node_destroy(&t);
		return obj;
	}
	return NULL;
}

