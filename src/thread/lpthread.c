#include "lpthread.h"
#include "dbg.h"

void *lpthread_routine(void * arg) {
	LOGT("pthread routine start");
	if (arg) {
		lpthread_t * context = (lpthread_t *)arg;
		LOGT("handle context: '%s'", context->name);
		lpthread_lock(context);
		context->in_progress = true;
		lpthread_unlock(context);

		if (context->running) {
			mLogT(context->name, "running");
			if (context->start) {
				mLogT(context->name, "exec start callback");
				if (!context->start(context->arg)) {
					mLogT(context->name, "abort requested by start callback");
					lpthread_lock(context);
					context->running = false;
					lpthread_unlock(context);
				}
			}
			while (context->running) {
				mLogT(context->name, "exec callback");
				if (context->callback &&
					context->callback(context->arg)) {
					continue;
				}
				mLogT(context->name, "aborted");
				lpthread_lock(context);
				context->running = false;
				lpthread_unlock(context);
				break;
			}
			mLogT(context->name, "loop finished");
			if (context->stop) {
				mLogT(context->name, "exec stop callback");
				context->stop(context->arg);
				mLogT(context->name, "stop callback returned");
			}
		}

		lpthread_lock(context);
		context->in_progress = false;
		lpthread_unlock(context);
	}
	LOGT("pthread routine start");
	return arg;
}

pthread_mutex_t * lpthread_create_mutex(int arg) {
	pthread_mutex_t * mutex = NULL;
	pthread_mutexattr_t attr;
	if ((pthread_mutexattr_init(&attr) == 0) &&
		(pthread_mutexattr_settype(&attr, arg) == 0)
		&& (mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)))) {
		if ((pthread_mutex_init(mutex, &attr) == 0) &&
			(pthread_mutexattr_destroy(&attr) == 0)) {
			return mutex;
		}
		free(mutex);
	}
	return mutex;
}

void lpthread_destroy_mutex(pthread_mutex_t ** mutex) {
	if (mutex && *mutex) {
		pthread_mutex_destroy(*mutex);
		destroy((void *)(*mutex));
		*mutex = NULL;
	}
}

lpthread_t * lpthread_get_context(
	_Bool (*start)(void *),
	_Bool (*callback)(void *),
	void (*stop)(void *),
	char * name, void * arg) {
	lpthread_t * context = (lpthread_t *)malloc(sizeof(lpthread_t));
	if (context) {
		if ((context->mutex = lpthread_create_mutex(
			PTHREAD_MUTEX_ERRORCHECK))) {
			context->name = name;
			context->running = false;
			context->in_progress = false;
			context->start = start;
			context->callback = callback;
			context->stop = stop;
			context->arg = arg;
			return context;
		}
		free(context);
	}
	return context;
}

void lpthread_destroy(lpthread_t ** context) {
	if (context && *context) {
		lpthread_destroy_mutex(&(*context)->mutex);
		destroy((void **)context);
	}
}

void lpthread_lock(lpthread_t * context) {
	if (context) {
		int ret;
		mLogD(context->name, "lock mutex");
		if ((ret = pthread_mutex_lock(context->mutex)) != 0) {
			mLogE(context->name, "lock mutex failed; ERROR: %d", ret);
			return;
		}
		mLogD(context->name, "mutex locked");
	}
}

void lpthread_unlock(lpthread_t * context) {
	if (context) {
		int ret;
		mLogD(context->name, "unlock mutex");
		if ((ret = pthread_mutex_unlock(context->mutex)) != 0) {
			mLogE(context->name, "unlock mutex failed; ERROR: %d", ret);
			return;
		}
		mLogD(context->name, "mutex unlocked");
	}
}
//
//_Bool lpthread_is_running(lpthread_t * context) {
//	_Bool ret = false;
//	if (context) {
//		lpthread_lock(context);
//		ret = context->running;
//		lpthread_unlock(context);
//	}
//	return ret;
//}

int lpthread_run(lpthread_t * context) {
	int ret = -1;
	if (context) {
		ret = 1;
		if (!context->running && !context->in_progress) {
			mLogD(context->name, "running loop thread");
			context->running = true;
			mLogD(context->name, "create pthread");
			if ((ret = pthread_create(&context->thread, NULL,
				lpthread_routine, (void *)context)) != 0) {
				mLogE(context->name, "create pthread failed; ERROR: %d", ret);
				return ret;
			}
			mLogD(context->name, "create success");
			return ret;
		}
		mLogW(context->name, "loop thread already in progress");
	}
	return ret;
}

void lpthread_abort(lpthread_t * context) {
	if (context && context->running && context->in_progress) {
		mLogD(context->name, "abort request");
		lpthread_lock(context);
		context->running = false;
		lpthread_unlock(context);
		mLogD(context->name, "wait for loop thread");
		while (context->in_progress);
		mLogD(context->name, "join loop thread");
		pthread_join(context->thread, (void **)(&context));
		mLogD(context->name, "abort requested");
	}
}
