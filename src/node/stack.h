#ifndef STACK_H
#define STACK_H

#include "node.h"

void stack_push(s_node_t ** tos, void * object);
void * stack_pop(s_node_t ** tos);

#endif // STACK_H
