#include "test_object.h"


test_object_t * create_test_object(int id) {
	test_object_t * object = malloc(sizeof(test_object_t));
	if (object) {
		object->id = id;
		object->size = randint(8, 64);
		object->str = randstr(object->size);
		object->size = strlen(object->str);
		sha256((uint8_t *)object->str, object->size, object->sha);
		object->addr = (uint64_t)object;
	}
	return object;
}

void check_object(test_object_t * object) {
	if (object) {
		uint64_t addr = (uint64_t)object;
		uint8_t sha[32];
		char shastr[65];
		char obj_shastr[65];
		size_t size = strlen(object->str);
		sha256((uint8_t *)object->str, size, sha);
		fstrdata(shastr, sha, 32);
		fstrdata(obj_shastr, object->sha, 32);
		LOGD("id: %d", object->id);
		printf(F_CYAN
			"id  : " F_CYAN "%d;"C_ENDLINE
			"text: " F_WHITE "\"" F_CYAN "%s" F_WHITE "\";\n"
			, object->id, object->str);

		if (addr != object->addr) {
			printf("addr: " C_BOLD F_RED "%p:%p;" C_ENDLINE, (void *)addr, (void *)object->addr);
		} else {
			printf("addr: " F_CYAN "%p:%p;" C_ENDLINE, (void *)addr, (void *)object->addr);
		}

		if (size != object->size) {
			printf("size: " C_BOLD F_RED "%lu:%lu;" C_ENDLINE, size, object->size);
		} else {
			printf("size: " F_CYAN "%lu:%lu;" C_ENDLINE, size, object->size);
		}

		if (!memcmp(object->sha, sha, 32)) {
			printf("sha256sum: \n"
				C_BOLD F_RED
				"expected: %s\n"
				"actual  : %s\n"
				C_ENDLINE
				, shastr, obj_shastr);
		} else {
			printf("sha256sum: \n"
				F_CYAN
				"expected: %s\n"
				"actual  : %s\n"
				C_ENDLINE
				, shastr, obj_shastr);
		}
	}
}

void destroy_object(void ** object) {
	printf(F_YELLOW "removing object" C_ENDLINE);

	if (object && *object) {
		print_object((test_object_t *)*object, "\0", "+- \0");
		if (((test_object_t *)*object)->str) {
			free(((test_object_t *)*object)->str);
		}
		free((void *)*object);
		*object = NULL;
	} else {
		printf("object=" C_BOLD F_RED "(null)" C_ENDLINE);
	}
}

void print_object(test_object_t * object, char * str1, char * str2) {
	if (object) {
		char shastr[65];
		fstrdata(shastr, object->sha, 32);
		printf(C_BOLD F_CYAN
			C_BOLD F_WHITE "%s" C_DEFAULT F_MAGENTA "object" C_BOLD F_WHITE "[" F_CYAN "%p" F_WHITE "] " C_ENDLINE
			C_BOLD F_WHITE "%s" C_DEFAULT F_MAGENTA "id  " C_BOLD F_WHITE ": " F_CYAN "%d" F_WHITE ";" C_ENDLINE
			C_BOLD F_WHITE "%s" C_DEFAULT F_MAGENTA F_MAGENTA "text" C_BOLD F_WHITE ": " F_WHITE "\"" F_CYAN "%s" F_WHITE "\";" C_ENDLINE
			C_BOLD F_WHITE "%s" C_DEFAULT F_MAGENTA F_MAGENTA "size" C_BOLD F_WHITE ": " F_CYAN "%lu" F_WHITE ";" C_ENDLINE
			C_BOLD F_WHITE "%s" C_DEFAULT F_MAGENTA F_MAGENTA "sha " C_BOLD F_WHITE ": " F_CYAN "%s" F_WHITE ";" C_ENDLINE
			"\n"
			, str1, object
			, str2, object->id
			, str2, object->str
			, str2, object->size
			, str2, shastr);
	}
}
