#ifndef IOFS_H
#define IOFS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "dbg.h"

typedef enum {
	IOFS_DIR,
	IOFS_CHR,
	IOFS_BLK,
	IOFS_REG,
	IOFS_FIFO,
	IOFS_LNK,
	IOFS_SOCK,
	IOFS_UNKNOWN
} iofs_mode_t;

typedef struct _iofs_item_t {
	char * i_pathname;
	iofs_mode_t i_mode;
	struct stat i_stat;
	void * i_object;
} iofs_item_t;

iofs_mode_t iofs_mode_from_stat(struct stat * ptr);
iofs_mode_t iofs_mode_from_dirent(struct dirent * ptr);
iofs_mode_t iofs_get_mode(const char * pathname);
iofs_item_t * iofs_create_item(const char * pathname);
void iofs_destroy_item(iofs_item_t ** item);

#endif // IOFS_H
