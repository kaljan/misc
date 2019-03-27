#include "iofs.h"
#include "str.h"
#include "dbg.h"

iofs_mode_t iofs_mode_from_stat(struct stat * ptr) {
	if (ptr) {
		if (S_ISDIR(ptr->st_mode)) {
			return IOFS_DIR;
		}
		if (S_ISCHR(ptr->st_mode)) {
			return IOFS_CHR;
		}
		if (S_ISBLK(ptr->st_mode)) {
			return IOFS_BLK;
		}
		if (S_ISREG(ptr->st_mode)) {
			return IOFS_REG;
		}
		if (S_ISFIFO(ptr->st_mode)) {
			return IOFS_FIFO;
		}
		if (S_ISLNK(ptr->st_mode)) {
			return IOFS_LNK;
		}
		if (S_ISSOCK(ptr->st_mode)) {
			return IOFS_SOCK;
		}
	}
	return IOFS_UNKNOWN;
}

iofs_mode_t iofs_mode_from_dirent(struct dirent * ptr) {
	if (ptr) {
		switch (ptr->d_type) {
		case DT_DIR: {
			LOGD("MODE: DIR");
			return IOFS_DIR;
		}
		case DT_CHR: {
			LOGD("MODE: CHR");
			return IOFS_CHR;
		}
		case DT_BLK: {
			LOGD("MODE: BLK");
			return IOFS_BLK;
		}
		case DT_REG: {
			LOGD("MODE: REG");
			return IOFS_REG;
		}
		case DT_FIFO: {
			LOGD("MODE: FIFO");
			return IOFS_FIFO;
		}
		case DT_LNK: {
			LOGD("MODE: LNK");
			return IOFS_LNK;
		}
		case DT_SOCK: {
			LOGD("MODE: SOCK");
			return IOFS_SOCK;
		}
		default: break;
		}
	}
	LOGD("MODE: UNKNOWN");
	return IOFS_UNKNOWN;
}

iofs_mode_t iofs_get_mode(const char * pathname) {
	struct stat fs;
	if (pathname && strlen(pathname) && (stat(pathname, &fs) == 0)) {
		LOGD("Get mode. path: \"%s\"", pathname);
		return iofs_mode_from_stat(&fs);
	}
	LOGE("Get mode failed");
	return IOFS_UNKNOWN;
}

iofs_item_t * iofs_create_item(const char * pathname) {
	iofs_item_t * item = NULL;
	struct stat fs;
	if (pathname && (strlen(pathname) > 0) && (stat(pathname, &fs) == 0) &&
		(item = (iofs_item_t *)malloc(sizeof(iofs_item_t)))) {
		memset(item, 0, sizeof(iofs_item_t));
		memmove((void *)&item->i_stat, (const void *)&fs, sizeof(fs));
		item->i_pathname = strdup(pathname);
		item->i_mode = iofs_mode_from_stat(&fs);
	}
	return item;
}

void iofs_destroy_item(iofs_item_t ** item) {
	if (item && *item) {
		destroy((void **)(&(*item)->i_pathname));
		free((void *)*item);
		*item = NULL;
	}
}
