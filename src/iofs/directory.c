#include "directory.h"
#include "dbg.h"
#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static inline _Bool verify_direcroty(iofs_mode_t mode) {
	LOGD("Verify directory");
	if (mode == IOFS_DIR) {
		LOGI("Directory OK");
		return true;
	}
	LOGE("This is not a directory");
	return false;
}

directory_t * directory_get_instance(const char * path) {
	directory_t * instance = NULL;
	LOGD("Creating directory instance: %s", path);
	if (verify_direcroty(iofs_get_mode(path)) &&
		(instance = (directory_t *)malloc(sizeof(directory_t)))) {
		memset(instance, 0, sizeof(directory_t));
		instance->path = strdup(path);
	}
	return instance;
}

void directory_destroy_instance(directory_t ** instance) {
	if (instance && *instance) {
		destroy((void **)(&(*instance)->path));
		list_empty(&(*instance)->items);
		destroy((void **)instance);
	}
}

iofs_item_t * iofs_set_directofy(const char * pathname) {
	iofs_item_t * item = NULL;
	directory_t * directory = directory_get_instance(pathname);
	if (directory && (item = iofs_create_item(pathname))) {
		item->i_object = (void *)directory;
		return item;
	}
	iofs_destroy_item(&item);
	directory_destroy_instance(&directory);
	return item;
}

directory_t * iofs_get_dir(iofs_item_t * item) {
	if (item && (item->i_mode == IOFS_DIR)) {
		return (directory_t *)item->i_object;
	}
	return NULL;
}

int directory_open(directory_t * instance) {
	int ret = -1;
	if (instance) {
		ret = 0;
		if ((instance->stream = opendir(instance->path)) == NULL) {
			LOGE("Failed to open directory \"%s\". ERROR(%d): %s"
				, instance->path, errno, strerror(errno));
			ret = -1;
		}
		instance->error_no = errno;
		errno = 0;
	}
	return ret;
}

void directory_close(directory_t * instance) {
	if (instance && instance->stream) {
		if (closedir(instance->stream) != 0) {
			LOGE("Failed to close directory \"%s\". ERROR(%d): %s"
				, instance->path, errno, strerror(errno));
		}
	}
}

int directory_scan(directory_t * instance) {
	int ret = -1;
	if (instance && instance->stream) {
		LOGD("Scanning directory \"%s\"", instance->path);

		while ((instance->entry = readdir(instance->stream))) {
			iofs_item_t * item = NULL;
			char pathname[4096];
			sprintf(pathname, "%s/%s", instance->path, instance->entry->d_name);
			printf("file: %s\n", pathname);
			item = iofs_create_item(pathname);
			list_push_back(&instance->items, item);
		}
	}
	return ret;
}


//int sdir(char *path)
//{
//	int ret;
//	struct stat st;
//	DIR *dr;
//	struct dirent *de;
//
//	if ((ret = stat(path, &st)) != 0) {
//		printf("[%d] %s\n", ret, strerror(errno));
//		return -1;
//	}
//
//	if ((st.st_mode & S_IFMT) != S_IFDIR) {
//		printf("%s is not directory\n", path);
//		return -1;
//	}
//
//	dr = opendir(path);
//	if (dr == 0) {
//		printf("Open directory failed: %s\n", strerror(errno));
//		return -1;
//	}
//
//	errno = 0;
//	while ((de = readdir(dr)) != 0) {
//
//		wrc = scvstr(de->d_name, trfname);
//
//		if (de->d_name[0] == '.') {
//			continue;
//		}
//
//		if (de->d_type == DT_DIR) {
//			if (de->d_name[0] != '.') {
//				printf("[%3li] %s/\n", strlen(de->d_name), de->d_name);
//			}
//		} else if (de->d_type == DT_REG) {
//			printf("[%3li] %s@\n", strlen(de->d_name), de->d_name);
//		}
//		if (wrc > 0) {
//			printf("[%3d] %s\n",wrc, trfname);
//		}
//		printf("\n");
//	}
//
//	printf("\n%s\n", strerror(errno));
//
//	closedir(dr);
//
//	return 0;
//}


//		DT_REG;
/* = get_entry_mode(instance->entry->d_type)*/
//iofs_mode_t get_entry_mode(uint8_t mode) {
//	switch (mode) {
//	case DT_DIR:
//		return IOFS_DIR;
//	case DT_CHR:
//		return IOFS_CHR;
//	case DT_BLK:
//		return IOFS_BLK;
//	case DT_REG:
//		return IOFS_REG;
//	case DT_FIFO:
//		return IOFS_FIFO;
//	case DT_LNK:
//		return IOFS_LNK;
//	default:
//		return IOFS_UNKNOWN;
//	}
//	return IOFS_UNKNOWN;
//}
// (instance->d_stat.st_mode & S_IFMT) != S_IFDIR
//					list_push_back(&instance->items,
//						(void *)iofs_create_item(
//							instance->path,
//							instance->entry->d_name, mode, directory_get_instance()));
