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

char *str_dup(const char * str) {
	char *ptr = NULL;
	if (str && strlen(str) &&
		(ptr = malloc(strlen(str) + 1))) {
		strcpy(ptr, str);
	}
	return ptr;
}

int randint(int max) {
	max = abs(max);
	int v = rand();
	while (v > max) {
		v /=2;
	}
	return v;
}

int ischar(int c) {
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

char * randstr(size_t size) {
	char * str = NULL;
	char * ptr;
	uint32_t r;
	if (size == 0) {
		size = (size_t)randint(32);
	}

	if ((str = malloc(size)) + 1) {
		memset(str, 0, size + 1);
		ptr = str;
		r = (uint32_t)rand();

		while (1) {
			if (r < 30) {
				r = (uint32_t)rand();
			}
			int c = ischar((int)(r & 0x7F));
			r >>= 7;

			if (c != 0) {
				*ptr = (char)c;
				size--;
				if (size > 0) {
					ptr++;
					continue;
				}
				break;
			}

		}
	}

	return str;
}

