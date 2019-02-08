#include "dlist.h"

void push_back(d_node_t **tail, void *object) {
	if (tail) {
		d_node_t * node = d_node_create(object, (void *)(*tail), NULL);
		if (node) {
			if (*tail) {
				(*tail)->next = node;
			}
			*tail = node;
		}
	}
}

void pop_back(d_node_t **tail) {
	if (tail && *tail) {
		if ((*tail)->prev) {
			*tail = (*tail)->prev;
			d_node_destroy((d_node_t **)(&(*tail)->next));
			return;
		}
		d_node_destroy(tail);
	}
}

void push_front(d_node_t **head, void *object) {
	if (head) {
		d_node_t * node = d_node_create(object, NULL, (void *)(*head));
		if (node) {
			if (*head) {
				(*head)->prev = node;
			}
			*head = node;
		}
	}
}

void pop_front(d_node_t **head) {
	if (head && *head) {
		if ((*head)->next) {
			*head = (*head)->next;
			d_node_destroy((d_node_t **)(&(*head)->prev));
			return;
		}
		d_node_destroy(head);
	}
}

int get_size(d_node_t * head) {
	int size = -1;
	if (head) {
		size = 0;
		while(head) {
			head = head->next;
			size++;
		}
	}
	return size;
}

void empty(d_node_t ** tail) {
	if (tail) {
		while(*tail) {
			pop_back(tail);
		}
	}
}
