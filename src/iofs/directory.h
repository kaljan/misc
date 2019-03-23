#ifndef IOFS_DIRECTORY_H
#define IOFS_DIRECTORY_H

#include <dirent.h>
#include "node.h"

typedef struct _directory_t {
	char * d_path;
	int f_des;
	DIR * d_stream;
	struct dirent * d_entry;
	d_node_t * head;
	d_node_t * tail;
} directory_t;

#endif // IOFS_DIRECTORY_H
