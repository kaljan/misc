#ifndef LPTHREAD_H
#define LPTHREAD_H

#include <pthread.h>
#include <str.h>

typedef struct _lpthread_t {
	_Bool running;
	_Bool in_progress;
	char * name;
	pthread_t thread;
	pthread_mutex_t * mutex;
	_Bool (*start)(void *);
	_Bool (*callback)(void *);
	void (*stop)(void *);
	void * arg;
} lpthread_t;

pthread_mutex_t * lpthread_create_mutex(int arg);
void lpthread_destroy_mutex(pthread_mutex_t ** mutex);
lpthread_t * lpthread_get_context(
	_Bool (*start)(void *),
	_Bool (*callback)(void *),
	void (*stop)(void *),
	char * name, void * arg);
void lpthread_destroy(lpthread_t ** context);
void lpthread_lock(lpthread_t * context);
void lpthread_unlock(lpthread_t * context);
int lpthread_run(lpthread_t * context);
void lpthread_abort(lpthread_t * context);

#endif // LPTHREAD_H
