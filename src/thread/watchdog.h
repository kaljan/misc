#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "lpthread.h"

typedef struct _watchdog_t {
	int delay;
	int clear;
	lpthread_t * thread;
	pthread_mutex_t * mutex;
	void (*callback)(void *);
	void * arg;
} watchdog_t;

watchdog_t * watchdog_create(int delay, void (*callback)(void *));
void watchdog_destroy(watchdog_t ** context);
int watchdog_run(watchdog_t * context);
void watchdog_abort(watchdog_t * context);
void watchdog_clear(watchdog_t * context, _Bool abrt);

#endif // WATCHDOG_H
