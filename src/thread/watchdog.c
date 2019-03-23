#include "watchdog.h"
#include <stdlib.h>

_Bool watchdog_callback(void *arg) {
	if (arg) {
		watchdog_t * context = (watchdog_t *)arg;
		if (context->delay > 0) {
			pthread_mutex_lock(context->mutex);
			context->delay--;
			pthread_mutex_unlock(context->mutex);
			usleep(1000);
			return true;
		}
		if (context->callback) {
			context->callback();
		}
	}
	return false;
}

watchdog_t * watchdog_create(int delay, void (*callback)(void *), void * arg) {
	watchdog_t * context = (watchdog_t *)malloc(sizeof(watchdog_t));
	if (context) {
		if ((context->mutex = lpthread_create_mutex(PTHREAD_MUTEX_DEFAULT))) {
			if ((context->thread = lpthread_get_context(
				NULL, watchdog_callback, NULL, strdup("watchdog"), context))) {
				context->callback = callback;
				context->arg = arg;
				context->delay = delay;
				context->clear = delay;
				return context;
			}
		}
		watchdog_destroy(&context);
	}
	return context;
}

void watchdog_destroy(watchdog_t ** context) {
	if (context && *context) {
		lpthread_destroy_mutex(&(*context)->mutex);
		lpthread_destroy(&(*context)->thread);
		free((void *)*context);
		*context = NULL;
	}
}

int watchdog_run(watchdog_t * context) {
	int ret = -1;
	if (context) {
		ret = lpthread_run(context->thread);
	}
	return ret;
}

void watchdog_abort(watchdog_t * context) {
	if (context) {
		lpthread_abort(context->thread);
	}
}

void watchdog_clear(watchdog_t * context, _Bool abrt) {
	if (abrt) {
		watchdog_abort(context);
		return;
	}

	if (context) {
		pthread_mutex_lock(context->mutex);
		context->delay = context->clear;
		pthread_mutex_unlock(context->mutex);
	}
}
