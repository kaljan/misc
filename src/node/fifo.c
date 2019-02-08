#include "fifo.h"
/**
 *
 * @param max
 * @return
 */
fifo_t * create_fifo(int max) {
	fifo_t * fifo = malloc(sizeof(fifo_t));
	if (fifo) {
		fifo->items = 0;
		fifo->max = max;
		fifo->empty = true;
		fifo->full = false;
	}
	return fifo;
}


/**
 *
 * @param fifo
 * @param destroy
 */
void destroy_fifo(fifo_t ** fifo, void (*destroy)(void **)) {
	if (fifo && *fifo) {
		flush_fifo(*fifo, destroy);
		free(*fifo);
		*fifo = NULL;
	}
}

/**
 *
 * @param fifo
 * @param object
 * @return
 */
_Bool fifo_write(fifo_t * fifo, void * object) {
	if (fifo && !fifo->full) {
		s_node_t * item = s_node_create(object, NULL);
		if (item) {
			if (fifo->in) {
				fifo->in->next = item;
			}
			fifo->in = item;
			fifo->empty = false;
			if (!fifo->out) {
				fifo->out = item;
			}
			fifo->items++;
			if (fifo->items >= fifo->max) {
				fifo->full = true;
			}
			return true;
		}
	}
	return false;
}

/**
 *
 * @param fifo
 */
void * fifo_read(fifo_t * fifo) {
	if (fifo && fifo->out) {
		void * object = fifo->out->object;
		s_node_t * item = fifo->out;
		fifo->out = fifo->out->next;
		if (!fifo->out) {
			fifo->in = NULL;
			fifo->empty = true;
		}
		fifo->items--;
		if (fifo->full) {
			fifo->full = false;
		}
		s_node_destroy(&item);
		return object;
	}
	return NULL;
}

/**
 *
 * @param fifo
 * @param destroy
 */
void flush_fifo(fifo_t * fifo, void (*destroy)(void **)) {
	if (fifo) {
		while (fifo->out) {
			void * object = fifo_read(fifo);
			if (destroy) {
				destroy(&object);
			}
		}
	}
}
