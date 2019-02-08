#include <stdlib.h>
#include "list.h"

void list_push_back(list_t ** list, void * object) {
	if (list) {
		if (*list) {
			push_back(&(*list)->tail, object);
			return;
		}
		if ((*list = (list_t *)malloc(sizeof(list_t)))) {
			push_back(&(*list)->tail, object);
			(*list)->head = (*list)->tail;
		}
	}
}

void list_pop_back(list_t ** list) {
	if (list && *list) {
		pop_back(&(*list)->tail);
		if (!(*list)->tail) {
			free((void *)(*list));
			*list = NULL;
		}
	}
}

void list_push_front(list_t ** list, void * object) {
	if (list) {
		if (*list) {
			push_front(&(*list)->head, object);
			return;
		}
		if ((*list = (list_t *)malloc(sizeof(list_t)))) {
			push_front(&(*list)->head, object);
			(*list)->tail = (*list)->head;
		}
	}
}

void list_pop_front(list_t ** list) {
	if (list && *list) {
		pop_front(&(*list)->head);
		if (!(*list)->head) {
			free((void *)(*list));
			*list = NULL;
		}
	}
}

void * list_get_item(list_t * list) {
	if (list) {
		if (!list->curr) {
			list->curr = list->head;
		}

		if (list->curr) {
			return list->curr->object;
		}
	}
	return NULL;
}

void * list_get_head(list_t * list) {
	if(list && list->head) {
		return list->head->object;
	}
	return NULL;
}

void * list_get_tail(list_t * list) {
	if(list && list->tail) {
		return list->tail->object;
	}
	return NULL;
}

void * list_get_next(list_t * list) {
	if (list) {
		if(list->curr) {
			list->curr = (d_node_t *)list->curr->next;
			if (list->curr) {
				return list->curr->object;
			}
			return NULL;
		}

		if (list->head) {
			list->curr = list->head;
			return list->curr->object;
		}
	}
	return NULL;
}

void * list_get_prev(list_t * list) {
	if (list) {
		if(list->curr) {
			list->curr = (d_node_t *)list->curr->prev;
			if (list->curr) {
				return list->curr->object;
			}
			return NULL;
		}

		if (list->tail) {
			list->curr = list->tail;
			return list->curr->object;
		}
	}
	return NULL;
}

void list_remove_item(list_t ** list) {
	if (list && (*list)->curr) {
		if (!((*list)->curr)->prev) {
			(*list)->curr = NULL;
			list_pop_front(list);
			return;
		}

		if (!((*list)->curr)->next) {
			(*list)->curr = NULL;
			list_pop_back(list);
			return;
		}
		d_node_t * curr = (*list)->curr;
		(*list)->curr = ((*list)->curr)->next;
		((d_node_t *)(curr->next))->prev = curr->prev;
		((d_node_t *)(curr->prev))->next = curr->next;
		free(curr);
	}
}

int list_size(list_t * list) {
	if (list) {
		return get_size(list->head);
	}
	return -1;
}

void list_empty(list_t ** list) {
	if (list && *list) {
		empty(&(*list)->head);
		free(*list);
		*list = NULL;
	}
}
