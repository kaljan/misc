#ifndef LIST_H
#define LIST_H

#include "dlist.h"

typedef struct _list_t {
	d_node_t * head;
	d_node_t * curr;
	d_node_t * tail;
} list_t;

void list_push_back(list_t ** list, void * object);
void list_pop_back(list_t ** list);
void list_push_front(list_t ** list, void * object);
void list_pop_front(list_t ** list);
void * list_get_item(list_t * list);
void * list_get_head(list_t * list);
void * list_get_tail(list_t * list);
void * list_get_next(list_t * list);
void * list_get_prev(list_t * list);
void list_remove_item(list_t ** list);
int list_size(list_t * list);
void list_empty(list_t ** list);

#endif // LIST_H
