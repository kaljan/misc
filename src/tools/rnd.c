#include "rnd.h"

void initrand(void) {
	struct timeval t;
	if (gettimeofday(&t, NULL) != 0) {
		return;
	}
	srand((uint32_t)(t.tv_sec ^ t.tv_usec));
}

int randint(int start, int end) {
	if (start < end) {
		return (rand() % (abs(end - start) + 1)) + start;
	}

	if (start > end) {
		return (rand() % (abs(end - start) + 1)) + end;
	}
	return start;
}

static int ischar(int c) {
	if (c >= '0' && c <= '9') {
		return c;
	}
	if (c >= 'A' && c <= 'Z') {
		return c;
	}
	if (c >= 'a' && c <= 'z') {
		return c;
	}
	if (c == '_') {
		return c;
	}
	return 0;
}

void frandstr(char * ptr, size_t size) {
	if (ptr && size > 2) {
		uint32_t r;
		int c;
		r = (uint32_t)rand();
		while (size > 0) {
			if (r < 20) {
				r = (uint32_t)rand();
			}

			c = ischar((int)(r & 0x7F));
			r >>= 7;

			if (c != 0) {
				*ptr = (char)c;
				size--;
				ptr++;
			}
		}
		*ptr = 0;
	}
}

char * randstr(size_t size) {
	char * str = malloc(size + 1);
	frandstr(str, size);
	return str;
}

void franddata(uint8_t * data, size_t size) {
	if (data && size > 0) {
		while (size > 0) {
			int s = (size < 4) ? size : 4;
			int r = rand();
			memmove(data, &r, s);
			data += s;
			size -= s;
		}
	}
}

uint8_t * randdata(size_t size) {
	uint8_t * data = malloc(size);
	franddata(data, size);
	return data;
}

