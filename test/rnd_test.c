#include "str.h"
#include "rnd.h"
//extern void sha256(const uint8_t *data, size_t size, uint8_t *md);

int ran_dint(int start, int end) {
	if (start < end) {
		return (rand() % (abs(end - start) + 1)) + start;
	}

	if (start > end) {
		return (rand() % (abs(end - start) + 1)) + end;
	}
	return start;
}

int randint_test() {
	initrand();
	for (int n = 0; n < 100; n++) {
		int start = ran_dint(-512, 512);
		int end = ran_dint(-512, 512);
		if (n == 97) {
			start = 0;
			end = -1;
		}
		if (n == 98) {
			start = -1;
			end = 0;
		}
		if (n == 99) {
			start = end;
		}
		printf(
			"test  : \e[1;32m%5d\e[0m;\n"
			"start : \e[1;32m%5d\e[0m;\n"
			"end   : \e[1;32m%5d\e[0m;\n"
			,n , start, end);
		for (int i = 0; i < 100; i++) {
			int value = ran_dint(start, end);
			if ((i != 0) && ((i % 10) == 0)) {
				printf("\n");
			}

			if (start < end) {
				if (value < start || value > end) {
					printf("\e[1;31m%6d\e[0m, ", value);
					continue;
				}
			} else if (start > end) {
				if (value > start || value < end) {
					printf("\e[1;31m%6d\e[0m, ", value);
					continue;
				}
			} else {
				printf("\e[1;31m%6d\e[0m, ", value);
				continue;
			}
			if (value == start) {
				printf("\e[1;35m%6d\e[0m, ", value);
				continue;
			}
			if (value == end) {
				printf("\e[1;33m%6d\e[0m, ", value);
				continue;
			}
			printf("\e[36m%6d\e[0m, ", value);
		}
		printf("\n\n");
	}
	return 0;
}


int randstr_test() {
	for (int i = 0; i < 100; i++) {
		size_t size = (size_t)randint(8, 64);
		char * str = randstr(size);
		size_t real_size = strlen(str);
		if (real_size == size) {
			printf("i: %2d; size: %2lu|%2lu; str: \"%s\"\n", i, real_size, size, str);
		} else {
			printf("\e[1;31mi: %2d; size: %2lu|%2lu; str: \"%s\"\e[0m\n", i, real_size, size, str);
		}
		free(str);
	}
	printf("sleep 1 s\n");
	sleep(1);
	printf("finished\n");
	return 0;
}

int rnddata_test() {
	initrand();
	for (int i = 0; i < 100; i++) {
		size_t size = randint(8, 64);
		uint8_t * data = randdata(size);
		char * str = strdata(data, size);
		size_t s = (strlen(str) / 2) + randint(-1, 1);
		if (size != s) {
			printf("\e[1;31m%2d; %2lu;%2lu; %s\e[0m\n", i, size, s, str);
		} else {
			printf("\e[36m%2d; %2lu;%2lu; %s\e[0m\n", i, size, s, str);
		}
		free(str);
	}
	printf("sleep 1 s\n");
	sleep(1);
	printf("finished\n");
	return 0;
}
