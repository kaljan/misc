#include "buffer.h"
#include <string.h>
#include <stdio.h>
/**
 *
 * @param size
 * @return
 */
buffer_t * create_buffer(size_t size) {
	buffer_t * buf = malloc(sizeof(buffer_t));
	if (buf) {
		if (!(buf->data = malloc(size))) {
			free(buf);
			return NULL;
		}
		memset(buf->data, 0, size);
		buf->size = size;
		buf->bytes = 0;
	} else {
	}
	return buf;
}

/**
 *
 * @param buf
 */
void destroy_buffer(void ** object) {
	if (object && *object) {
		if (((buffer_t *)(*object))->data) {
			free(((buffer_t *)(*object))->data);
		}
		free(*object);
		*object = NULL;
	}
}

/**
 *
 * @param buf
 * @param data
 * @param size
 */
ssize_t write_buffer(buffer_t * buf, uint8_t * data, size_t size) {
	if (buf && data) {
		if  (buf->bytes < buf->size) {
			if (buf->bytes + size >= buf->size) {
				size = buf->size - buf->bytes;
			}
			memmove((void *)(buf->data + buf->bytes), (void *)data, size);
			buf->bytes += size;
			return size;
		}
		return 0;
	}
	return -1;
}

/**
 *
 * @param buf
 * @param data
 * @param size
 * @return
 */
ssize_t read_buffer(buffer_t * buf, uint8_t * data, size_t size) {
	if (buf && data) {
		if (size < buf->bytes) {
			memmove(data, buf->data, size);
			buf->bytes -= size;
			memmove(buf->data, (buf->data + size), buf->bytes);
			return (ssize_t)size;
		}
		size = buf->bytes;
		memmove(data, buf->data, size);
		buf->bytes = 0;
		return (ssize_t)size;
	}
	return -1;
}

/**
 *
 * @param object
 * @return
 */
s_node_t * create_double_buffer(void *object) {
	s_node_t * item = s_node_create(object, NULL);
	if (item) {
		if (!(item->next = s_node_create(object, item))) {
			s_node_destroy((s_node_t **)&item->next);
			return NULL;
		}
	}
	return item;
}

/**
 *
 * @param item
 * @param destroy
 */
void destroy_double_buffer(s_node_t ** item) {
	if (item && *item) {
		s_node_destroy((s_node_t **)(&(*item)->next));
		s_node_destroy(item);
	}
}

/**
 *
 * @param item
 */
void swap_buffer(s_node_t ** item) {
	if (item && *item && (*item)->next) {
		*item = (*item)->next;
	}
}


//	if (buf && data) {
//		if (buf->bytes > 0) {
//			if (size >= buf->bytes) {
//				size = buf->bytes;
//				memmove((void *)data, (void *)buf->data, size);
//				buf->bytes = 0;
//				return size;
//			}
//			memmove((void *)data, (void *)buf->data, size);
//			memmove((void *)buf->data, (void *)(buf->data + buf->bytes)
//				, buf->size - buf->bytes);
//			buf->bytes -= size;
//			return size;
//		}
//		return (ssize_t)buf->bytes;
//	}
//	return -1;
