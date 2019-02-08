#include "test_object.h"

void print_list_item(d_node_t * curr) {
	if (curr) {
//		printf(
//			FG_YELLOW "item" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE
//			FG_BOLD_WHITE "+- " FG_YELLOW "prev" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE
//			FG_BOLD_WHITE "+- " FG_YELLOW "next" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE
//			, (void *)curr, curr->prev, curr->next);
		return;
	}
//	printf(FG_YELLOW "curr" FG_BOLD_WHITE ": " FG_BOLD_RED "(null)" C_ENDLINE);
}

void print_list(d_node_t * curr) {
	while (curr) {
		print_list_item(curr);
		test_object_t * object = (test_object_t *)curr->object;
		print_object(object, "+- \0", "   +- \0");
		curr = curr->next;
	}
}

int push_back_test_objects(d_node_t ** tail, d_node_t ** head) {
	if (tail && head) {
		if(*tail) {
			while((*tail)->next) {
				*tail = (*tail)->next;
			}
			if (!(*head)) {
				*head = *tail;
				while ((*head)->prev) {
					*head = (*head)->prev;
				}
			}
		}

		int size = randint(8, 32);

		for (int i = 0; i < size; i++) {

			test_object_t * object = create_test_object(i);
//			printf(FG_YELLOW "create object" FG_BOLD_WHITE ":" FG_BOLD_CYAN " %d" C_ENDLINE, i);
			push_back(tail, (void *)object);

			if (!tail) {
//				printf("object=" FG_BOLD_RED "%p" C_ENDLINE, (void *)object);
				size = -1;
				break;
			}

			if (i == 0 && !(*head)) {
				*head = *tail;
			}

//			printf(FG_CYAN "object created" C_ENDLINE
//				FG_YELLOW "head" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE
//				FG_YELLOW "tail" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE "\n"
//				, (void *)(*head), (void *)(*tail));
		}
		return size;
	}
	return -1;
}

int pop_back_test_objects(d_node_t ** tail, d_node_t ** head) {
	if (head && tail) {
		int size = get_size(*head);
		int n = randint(1, size / 2);

//		printf(
//			"remove objects " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//			,n
//		);

		while (n > 0) {
//			destroy_object((void **)(&(*tail)->object));
			pop_back(tail);
			n--;
		}

		size = get_size(*head);

//		printf(
//			"actual size  " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//			,size
//		);
		return size;
	}
	return -1;
}

int push_front_test_objects(d_node_t ** tail, d_node_t ** head) {
	if (head && tail) {
		if(*head) {
			while((*head)->prev) {
				*head = (*head)->prev;
			}
			if (!(*tail)) {
				*tail = *head;
				while ((*tail)->prev) {
					*tail = (*tail)->prev;
				}
			}
		}

		int size = randint(8, 32);

		for (int i = 0; i < size; i++) {
			test_object_t * object = create_test_object(i);
//			printf(FG_YELLOW "create object" FG_BOLD_WHITE ":" FG_BOLD_CYAN " %d" C_ENDLINE, i);
			push_front(head, (void *)object);

			if (!(*head)) {
//				printf("object=" FG_BOLD_RED "%p" C_ENDLINE, (void *)object);
				size = -1;
				break;
			}

			if (i == 0 && !(*head)) {
				*tail = *head;
			}

//			printf(FG_CYAN "object created" C_ENDLINE
//				FG_YELLOW "head" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE
//				FG_YELLOW "tail" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE "\n"
//				, (void *)(*head), (void *)(*tail));
		}
		return size;
	}
	return -1;
}

int pop_front_test_objects(d_node_t ** tail, d_node_t ** head) {
	if (head && tail) {
		int size = get_size(*head);
		int n = randint(1, size / 2);

//		printf(
//			"remove objects " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//			,n
//		);

		while (n > 0) {
//			destroy_object((void **)(&(*head)->object));
			pop_front(head);
			n--;
		}

		size = get_size(*head);

//		printf(
//			"actual size  " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//			,size
//		);
		return size;
	}
	return -1;
}

void check_value(int expected, int actual, const char * str) {
	if (expected != actual) {
//		printf(
//			FG_MAGENTA "expected %s" FG_BOLD_WHITE ": " FG_BOLD_RED "%d" C_ENDLINE
//			FG_MAGENTA "actual   %s" FG_BOLD_WHITE ": " FG_BOLD_RED "%d" C_ENDLINE
//			,str ,expected, str, actual
//		);
	} else {
//		printf(
//			FG_MAGENTA "%s " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//			,str ,expected
//		);
	}
}

size_t get_list_full_size(d_node_t * head) {
	size_t size = 0;
	while (head) {
		size += (sizeof(*head));
		if (head->object) {
			size += sizeof((test_object_t)((*(test_object_t *)head->object)));
			if (((test_object_t *)head->object)->str) {
				size += strlen(((test_object_t *)head->object)->str) + 1;
			}
		}
		head = head->next;
	}
	return size;
}

int list_test(void) {
	d_node_t * head = NULL;
//	d_node_t * curr = NULL;
	d_node_t * tail = NULL;
	int iters, size;

	initrand();

//	printf(FG_BOLD_YELLOW "[%s:%d] Create new list" C_ENDLINE, __func__, __LINE__);
	iters = push_back_test_objects(&tail, &head);

	print_list(head);
	size = get_size(head);

	check_value(iters, size, "size");

//	printf(FG_BOLD_YELLOW "[%s:%d] Remove objects" C_ENDLINE, __func__, __LINE__);
	size = pop_back_test_objects(&tail, &head);

//	printf(
//		"actual size  " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//		,size
//	);

//	printf(FG_BOLD_YELLOW "[%s:%d] Add objects to head" C_ENDLINE, __func__, __LINE__);
	size = pop_front_test_objects(&tail, &head);

//	printf(
//		"actual size  " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//		,size
//	);

//	printf(FG_BOLD_YELLOW "[%s:%d] Remove objects from head" C_ENDLINE, __func__, __LINE__);
	size = push_front_test_objects(&tail, &head);

	print_list(head);

//	printf(
//		"actual size  " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d" C_ENDLINE
//		,size
//	);

	size = get_list_full_size(head);

//	printf(FG_BOLD_YELLOW "[%s:%d] Empty list" C_ENDLINE, __func__, __LINE__);
	empty(&tail);

	head = tail;

//	printf(FG_CYAN "object created" C_ENDLINE
//		FG_YELLOW "head" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE
//		FG_YELLOW "tail" FG_BOLD_WHITE ": " FG_BOLD_CYAN "%p" C_ENDLINE "\n"
//		, (void *)head, (void *) tail);

//	printf(
//		"full size  " FG_BOLD_WHITE ": " FG_BOLD_CYAN "%d bytes" C_ENDLINE
//		,size
//	);

	return 0;
}
