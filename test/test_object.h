#ifndef TEST_OBJECT_H
#define TEST_OBJECT_H

#include "str.h"
#include "rnd.h"
#include "dlist.h"

typedef struct _test_object_t {
	int id;
	char * str;
	size_t size;
	uint64_t addr;
	uint8_t sha[32];
} test_object_t;

extern void sha256(const uint8_t *data, size_t size, uint8_t *md);

test_object_t * create_test_object(int id);
void check_object(test_object_t * object);
void destroy_object(void ** object);
void print_object(test_object_t * object, char * str1, char * str2);

#endif
