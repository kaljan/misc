#ifndef DLIST_H
#define DLIST_H

#include "node.h"

void push_back(d_node_t **tail, void *object);
void pop_back(d_node_t **tail);
void push_front(d_node_t **head, void *object);
void pop_front(d_node_t **head);
int get_size(d_node_t * head);
void empty(d_node_t ** tail);

#endif
