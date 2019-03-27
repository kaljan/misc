#include "file.h"
#include "str.h"

char * f_split(const char * pathname, char ** path) {
	if (pathname && strlen(pathname) > 0) {
		char temp[4096];
		char * name;
		strcpy(temp, pathname);
		name = strrchr(temp, '/');
		if (name && strlen(name) > 1) {
			*name = 0;
			name++;
			*path = str_dup(temp);
			return str_dup(name);
		}
	}
	return NULL;
}

static inline _Bool file_verify(iofs_mode_t mode) {
	if ((mode == IOFS_REG) && (mode == IOFS_FIFO) &&
		(mode == IOFS_CHR) && (mode == IOFS_BLK)) {
		return true;
	}
	return false;
}

file_t * file_create_instance(const char * pathname, _Bool create) {
	file_t * instance = NULL;
	LOGD("Create file instance");
	if (file_verify(iofs_get_mode(pathname)) &&
		(instance = (file_t *)malloc(sizeof(file_t)))) {
		memset((void *)instance, 0, sizeof(file_t));
		instance->f_name = f_split(pathname, &instance->f_path);
		instance->f_pathname = str_dup(pathname);
		instance->f_des = -1;
		LOGI("Success!");
		LOGD("File name: %s", instance->f_name);
		LOGD("File path: %s", instance->f_path);
		LOGD("File p+n : %s", instance->f_pathname);
		return instance;
	}
	LOGE("Failed to create file instance");

	return instance;
}

void file_destroy_instance(file_t ** instance) {
	if (instance && * instance) {
		destroy((void **)(&(*instance)->f_name));
		destroy((void **)(&(*instance)->f_path));
		destroy((void **)(&(*instance)->f_pathname));
		destroy((void **)(&(*instance)->f_md));
		free((void *)*instance);
		*instance = NULL;
	}
}

iofs_item_t * iofs_set_file(const char * pathname, _Bool create) {
	iofs_item_t * item = NULL;
	file_t * file = file_create_instance(pathname, create);
	if (file && (item = iofs_create_item(pathname))) {
		item->i_object = (void *)file;
		return item;
	}
	iofs_destroy_item(&item);
	file_destroy_instance(&file);
	return item;
}

file_t * iofs_get_file(iofs_item_t * item) {
	if (item && (item->i_mode == IOFS_REG)) {
		return (file_t *)item->i_object;
	}
	return NULL;
}

int file_open(file_t * instance) {
	LOGD("Open file");
	if (instance) {
		LOGD("File name: %s", instance->f_pathname);
		if ((instance->f_des = open(
			instance->f_pathname, instance->f_flags)) < 0) {
			LOGE("Failed to open file \"%s\"; ERROR(%d) %s",
				instance->f_pathname, errno, strerror(errno));
			instance->error_no = errno;
			return instance->f_des;
		}
		LOGI("Success");
		return instance->f_des;
	}
	LOGE("Failed to open file. instance not found.");
	return -1;
}

void file_close(file_t * instance) {
	LOGD("Close file");
	if (instance) {
		LOGD("File name: %s", instance->f_pathname);
		if ((instance->f_des = close(instance->f_des)) < 0) {
			LOGE("Failed to close file \"%s\"; ERROR(%d) %s",
				instance->f_pathname, errno, strerror(errno));
			instance->error_no = errno;
			return;
		}
		LOGI("Success");
		return;
	}
	LOGE("Failed to close file. instance not found.");
}

ssize_t file_read(file_t * instance, uint8_t * data, size_t size) {
	ssize_t bytes;
	if (instance) {
		if ((bytes = read(instance->f_des, (void *)data, size)) < 0) {
			LOGE("Failed to read file \"%s\"; ERROR(%d) %s",
				instance->f_pathname, errno, strerror(errno));
			instance->error_no = errno;
		}
	}
	return bytes;
}

ssize_t file_write(file_t * instance, const void * data, size_t size) {
	ssize_t bytes;
	if (instance) {
		if ((bytes = write(instance->f_des, data, size)) < 0) {
			LOGE("Failed to read file \"%s\"; ERROR(%d) %s",
				instance->f_pathname, errno, strerror(errno));
			instance->error_no = errno;
		}
	}
	return bytes;
}
