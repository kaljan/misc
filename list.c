#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct _list_t {
	void * object;
	void * prev;
	void * next;
} list_t;

list_t * create_item(void * object, void * prev, void * next) {
	list_t * list = NULL;
	if (object && (list = malloc(sizeof(list)))) {
		list->object = object;
		list->prev = prev;
		list->next = next;
	}
	return list;
}

void remove_item(list_t **list) {
	if (list && *list) {
		free(*list);
		*list = NULL;
	}
}

void push_back(list_t **tail, void *object) {
	list_t * t = NULL;
	if (tail && (t = create_item(object, *tail, NULL)) && *tail) {
		(*tail)->next = (void *)t;
	}
	*tail = t;
}

void pop_back(list_t **tail) {
	list_t * t = NULL;
	if (tail && *tail) {
		if (!(t = (list_t*)((*tail)->prev))) {
			remove_item(tail);
			return;
		}
		remove_item((list_t **)(&t->next));
	}
	*tail = t;
}

void push_front(list_t **head, void *object) {
	list_t * h = NULL;
	if (head && (h = create_item(object, NULL, *head)) && *head) {
		(*head)->prev = (void *)h;
	}
	*head = h;
}

void pop_front(list_t **head) {
	list_t * h = NULL;
	if (head && *head) {
		if (!(h = (list_t *)((*head)->next))) {
			remove_item(head);
			return;
		}
		remove_item((list_t **)(&h->prev));
	}
	*head = h;
}

void print_item(list_t * item) {
	if (item) {
		printf(
			"item: %p\n"
			"\tobject : %p\n"
			"\tprev   : %p\n"
			"\tnext   : %p\n\n"
			, item
			, item->object
			, item->prev
			, item->next
		);
	}
}

typedef struct _strlist_t {
	list_t * head;
	list_t * curr;
	list_t * tail;
} strlist_t;

strlist_t * create_strlist(void) {
	strlist_t * list = malloc(sizeof(strlist_t));
	if (list) {
		memset(list, 0, sizeof(strlist_t));
	}
	return list;
}

void remove_strlist(strlist_t ** list) {
	if (list && *list) {
		free(*list);
		*list = NULL;
	}
}

void strlist_push_back(strlist_t ** list, const char * string) {
	if (list) {
		if (*list) {
			push_back((list_t**)(&(*list)->tail), (void *)string);
			if (!(*list)->head) {
				(*list)->head = (*list)->tail;
			}
			return;
		}
		if ((*list = create_strlist())) {
			push_back((list_t**)(&(*list)->tail), (void *)string);
			(*list)->head = (*list)->tail;
			return;
		}
	}
}

void strlist_pop_back(strlist_t ** list) {
	if (list && *list) {
		pop_back((list_t **)(&(*list)->tail));
		if (!(*list)->tail) {
			remove_strlist(list);
		}
	}
}

const char * get_str(strlist_t * list) {
	if (list) {
		if (!list->curr) {
			list->curr = list->head;
		}
		if (list->curr) {
			return (const char *)list->curr->object;
		}
	}
	return NULL;
}

const char * get_next(strlist_t * list) {
	if (list) {
		if (list->curr && ((list_t *)(list->curr))->next) {
			list->curr = ((list_t *)(list->curr))->next;
			return (const char *)list->curr->object;
		}
	}
	return NULL;
}


