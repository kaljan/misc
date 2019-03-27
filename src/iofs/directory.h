#ifndef FS_DIRECTORY_H
#define FS_DIRECTORY_H

#include "iofs.h"
#include "list.h"

typedef struct _directory_t {
	char * path;
	DIR * stream;
	struct dirent * entry;
	struct stat d_stat;
	_Bool hidden_visible;
	list_t * items;
	int error_no;
} directory_t;

directory_t * directory_get_instance(const char * path);
void directory_destroy_instance(directory_t ** instance);
iofs_item_t * iofs_set_directofy(const char * pathname);
int directory_open(directory_t * instance);
directory_t * iofs_get_dir(iofs_item_t * item);
void directory_close(directory_t * instance);
int directory_scan(directory_t * instance);

#endif // FS_DIRECTORY_H
