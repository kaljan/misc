#include "lpthread.h"
#include "test_object.h"
#include "dlist.h"
#include "dbg.h"

typedef struct _lpthread_test_t {
	int i;
	int max;
	int delay;
	_Bool abrt;
	char * tag;
	pthread_mutex_t * mutex;
	lpthread_t * lpthread;
	d_node_t * node;
} lpthread_test_t;

d_node_t * head;
d_node_t * curr;
d_node_t * tail;
_Bool abrt_test;
pthread_mutex_t * abrt_test_mutex;

_Bool start_routine(void * arg) {
	if (arg) {

	}
	return true;
}

_Bool loop_routine(void * arg) {
	if (arg) {
		lpthread_test_t * context = arg;
		mLogT(context->tag, "handle test data");
		pthread_mutex_lock(context->mutex);
		context->i++;
		pthread_mutex_unlock(context->mutex);
		if (context->i > context->max) {
			if (context->abrt) {
				mLogI(context->tag, "abort all threads requested");
				pthread_mutex_lock(abrt_test_mutex);
				abrt_test = true;
				pthread_mutex_unlock(abrt_test_mutex);
			}
			return false;
		}
		usleep(context->delay);
	}
	return true;
}

void stop_routine(void * arg) {

}

void destroy_lpthread_test_context(lpthread_test_t ** context) {
	if (context && *context) {
		LOGD("Destroy context");
		if ((*context)->lpthread) {
			LOGD("Destroy context thread");
			lpthread_destroy(&(*context)->lpthread);
		}

		if ((*context)->mutex) {
			LOGD("Destroy context mutex");
			lpthread_destroy_mutex(&(*context)->mutex);
		}
		LOGD("Remove context");
		free((void *)(*context));
		*context = NULL;
	}
}

lpthread_test_t * create_lpthread_test_context(
	char * tag, int max, int delay) {
	mLogD(tag, "create context");
	lpthread_test_t * context =
		(lpthread_test_t *)malloc(sizeof(lpthread_test_t));
	if (context) {
		mLogD(tag, "create context success");
		memset((void *)context, 0, sizeof(lpthread_test_t));
		do {
			mLogD(tag, "create context mutex");
			if (!(context->mutex = lpthread_create_mutex(PTHREAD_MUTEX_ERRORCHECK))) {
				break;
			}
			mLogD(tag, "create context thread");
			if (!(context->lpthread = lpthread_get_context(
				start_routine, loop_routine, stop_routine, tag, context))) {
				break;
			}
			context->tag = tag;
			context->max = max;
			context->delay = delay;
			context->abrt = false;
			return context;
		} while (0);
		mLogE(tag, "create context failed");
		destroy_lpthread_test_context(&context);
	}
	return context;
}

int create_new_test_thread(int id, int abrt_id) {
	int ret = -1;
	int max = randint(4, 16);
	int delay = randint(80, 320) * 100;
	char * tag = __fstr("THREAD#%d", id);
	LOGD("Create test thread context %s", tag);
	lpthread_test_t * context = create_lpthread_test_context(tag, max, delay);
	if (context) {
		mLogD(tag, "test thread context created.");
		push_back(&tail, context);
		if (tail) {
			mLogD(tag, "test thread succesfully added to list");
			ret = 0;
			if (head == NULL) {
				head = tail;
			}
			context->node = tail;
			if (id == abrt_id) {
				context->abrt = true;
			}
			return ret;
		}
		destroy_lpthread_test_context(&context);
	}
	free(tag);
	return ret;
}

int init_test(void) {
	initrand();
	int threads = randint(2, 16);
	int abrt_id = randint(0, threads);
	LOGI("threads: %d", threads);
	LOGI("abrt_id: %d", abrt_id);
	head = NULL;
	curr = NULL;
	tail = NULL;

	for (int i = 0; i < threads; i++) {
		LOGD("Create test #%d", i);
		if (create_new_test_thread(i, abrt_id) != 0) {
			LOGE("Create test #%d failed", i);
			return -1;
		}
	}
	return 0;
}

void deinit_test(void) {
	LOGD("Destroing all tests");
	while (head) {
		lpthread_test_t * context = (lpthread_test_t *)head->object;

		lpthread_abort(context->lpthread);
		pop_front(&head);
		destroy_lpthread_test_context(&context);
	}
}

int run_all(void) {
	d_node_t *item = head;
	int ret = 0;

	LOGD("Running all threads");

	while (item) {
		lpthread_test_t * context = (lpthread_test_t *)item->object;
		LOGD("Running test item: %s", context->lpthread->name);
		if (lpthread_run(context->lpthread) != 0) {
			LOGE("running %s failed", context->lpthread->name);
		} else {
			LOGD("running %s success", context->lpthread->name);
		}
		item = item->next;
	}
	LOGD("Running complete. %d thread in progress");
	return ret;
}

void test_loop(void) {
	while(1) {
		_Bool a;
		pthread_mutex_lock(abrt_test_mutex);
		a = abrt_test;
		pthread_mutex_unlock(abrt_test_mutex);
		if (a) {
			LOGI("Aboring all threads");
			break;
		}
	}
	return;
}

int loop_thread_test(void) {
	int ret = -1;

	head = NULL;
	curr = NULL;
	tail = NULL;
	abrt_test = false;

	LOGD("Initialization");

	if (!(abrt_test_mutex = lpthread_create_mutex(PTHREAD_MUTEX_ERRORCHECK))) {
		LOGE("create mutex failed");
		return -1;
	}

	ret = init_test();
	if (ret != 0) {
		LOGE("Initialization failed");
		return ret;
	}
	LOGD("Running test");
	ret = run_all();
	if (ret != 0) {
		LOGE("Running failed");
		return ret;
	}

	test_loop();

	deinit_test();
	return ret;
}
