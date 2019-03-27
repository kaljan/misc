#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>


char * str_dup(const char * str);
void fstrdata(char * str, uint8_t * data, size_t size);
char * strdata(uint8_t * data, size_t size);
void destroy(void ** object);
char * __fstr(const char * fmt, ...);

#endif // MISC_H
