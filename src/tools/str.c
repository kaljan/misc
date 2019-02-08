
#include <stdarg.h>
#include "str.h"

char *str_dup(const char * str) {
	char *ptr = NULL;
	if (str && strlen(str) &&
		(ptr = malloc(strlen(str) + 1))) {
		strcpy(ptr, str);
	}
	return ptr;
}

void fstrdata(char * str, uint8_t * data, size_t size) {
	if (str && data && size > 0) {
		while (size--) {
			sprintf(str, "%02x", *data);
			str += 2;
			data++;
		}
		*str=0;
	}
}

char * strdata(uint8_t * data, size_t size) {
	char * str = (char *)malloc(size + 1);
	fstrdata(str, data, size);
	return str;
}

void destroy(void ** object) {
	if (object && *object) {
		free(*object);
		object = NULL;
	}
}

char __f_str_buf[2048];

char * __fstr(const char * fmt, ...) {
	char * str = NULL;
	va_list vaptr;
	va_start(vaptr, fmt);
	vsprintf(__f_str_buf, fmt, vaptr);
	if ((str = (char *)malloc(strlen(__f_str_buf) + 1))) {
		strcpy(str, __f_str_buf);
	}
	va_end(vaptr);
	return str;
}
