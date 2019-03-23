#include "file.h"
#include "string.h"

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

file_t * file_create_instance(const char * pathname) {
	file_t * instance = NULL;
	struct stat f_stat;
	LOGD("stat file \"%s\"", pathname);
	if (stat(pathname, &f_stat) == 0) {
		LOGD("Create file instance");
		if ((instance = (file_t *)malloc(sizeof(file_t)))) {
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
	}

	return instance;
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

ssize_t file_write(file_t * instance, uint8_t * data, size_t size) {
	ssize_t bytes;
	if (instance) {
		if ((bytes = write(instance->f_des,
			(const void *)data, size)) < 0) {
			LOGE("Failed to read file \"%s\"; ERROR(%d) %s",
				instance->f_pathname, errno, strerror(errno));
			instance->error_no = errno;
		}
	}
	return bytes;
}
