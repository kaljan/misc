#ifndef TOOLS_RND_H
#define TOOLS_RND_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

void initrand(void);
int randint(int start, int end);
void frandstr(char * ptr, size_t size);
char * randstr(size_t size);
void franddata(uint8_t * data, size_t size);
uint8_t * randdata(size_t size);

#endif // TOOLS_RND_H
