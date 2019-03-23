#ifndef DBGTOOLS_H
#define DBGTOOLS_H

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#define STRENUM(s) #s
#define strbool(x) ((x) ? "true\0" : "false\0")

#ifdef COLORED_TERM
#define F_RED			"\e[31m"
#define F_GREEN			"\e[32m"
#define F_YELLOW		"\e[33m"
#define F_BLUE			"\e[34m"
#define F_MAGENTA		"\e[35m"
#define F_CYAN			"\e[36m"
#define F_WHITE			"\e[37m"

#define B_RED			"\e[41m"
#define B_GREEN			"\e[42m"
#define B_YELLOW		"\e[43m"
#define B_BLUE			"\e[44m"
#define B_MAGENTA		"\e[45m"
#define B_CYAN			"\e[46m"
#define B_WHITE			"\e[47m"

#define C_BOLD			"\e[1m"
#define C_DEFAULT		"\e[0m"
#define C_ENDLINE		"\e[0m\n"

#define D_SUCCESS		C_BOLD F_WHITE "[" F_GREEN "SUCCESS" F_WHITE "]" C_DEFAULT
#define D_FAILED		C_BOLD F_WHITE "[" F_RED   "FAILED"  F_WHITE "]" C_DEFAULT
#else
#define C_ENDLINE		"\n"
#endif


#define LOG_LEVEL_VERBOSE	0
#define LOG_LEVEL_INFO		1
#define LOG_LEVEL_THREAD	2
#define LOG_LEVEL_DEBUG		3
#define LOG_LEVEL_WARNING	4
#define LOG_LEVEL_ERROR		5

#define DEBOG_LOG_LEVEL		LOG_LEVEL_VERBOSE
#define LOG_LEVEL			LOG_LEVEL_VERBOSE

#if defined (DEBOG_LOG_LEVEL)
#	if (DEBOG_LOG_LEVEL == LOG_LEVEL_VERBOSE)
#	define mLogV(TAG, ...) __debug_log_print(LOG_LEVEL_VERBOSE, __func__, __LINE__, TAG, __VA_ARGS__)
#	else
#	define mLogV(TAG, ...) (void *)0
#	endif
#	if (DEBOG_LOG_LEVEL <= LOG_LEVEL_INFO)
#	define mLogI(TAG, ...) __debug_log_print(LOG_LEVEL_INFO, __func__, __LINE__, TAG, __VA_ARGS__)
#	else
#	define mLogI(TAG, ...) (void *)0
#	endif
#	if (DEBOG_LOG_LEVEL <= LOG_LEVEL_THREAD)
#	define mLogT(TAG, ...) __debug_log_print(LOG_LEVEL_THREAD, __func__, __LINE__, TAG, __VA_ARGS__)
#	else
#	define mLogT(TAG, ...) (void *)0
#	endif
#	if (DEBOG_LOG_LEVEL <= LOG_LEVEL_DEBUG)
#	define mLogD(TAG, ...) __debug_log_print(LOG_LEVEL_DEBUG, __func__, __LINE__, TAG, __VA_ARGS__)
#	else
#	define mLogD(TAG, ...) (void *)0
#	endif
#	if (DEBOG_LOG_LEVEL <= LOG_LEVEL_WARNING)
#	define mLogW(TAG, ...) __debug_log_print(LOG_LEVEL_WARNING, __func__, __LINE__, TAG, __VA_ARGS__)
#	else
#	define mLogW(TAG, ...) (void *)0
#	endif
#	if (DEBOG_LOG_LEVEL <= LOG_LEVEL_ERROR)
#	define mLogE(TAG, ...) __debug_log_print(LOG_LEVEL_ERROR, __func__, __LINE__, TAG, __VA_ARGS__)
#	else
#	define mLogE(TAG, ...) (void *)0
#	endif
#else
#	define mLogV(TAG, ...) (void *)0
#	define mLogT(TAG, ...) (void *)0
#	define mLogD(TAG, ...) (void *)0
#	define mLogW(TAG, ...) (void *)0
#	define mLogE(TAG, ...) (void *)0
#endif

#if defined (LOG_LEVEL)
#	if (LOG_LEVEL == LOG_LEVEL_VERBOSE)
#	define LOGV(...) __debug_print(LOG_LEVEL_VERBOSE, __func__, __LINE__, __VA_ARGS__)
#	else
#	define LOGV(...) (void *)0
#	endif
#	if (LOG_LEVEL <= LOG_LEVEL_INFO)
#	define LOGI(...) __debug_print(LOG_LEVEL_INFO, __func__, __LINE__, __VA_ARGS__)
#	else
#	define LOGI(...) (void *)0
#	endif
#	if (LOG_LEVEL <= LOG_LEVEL_THREAD)
#	define LOGT(...) __debug_print(LOG_LEVEL_THREAD, __func__, __LINE__, __VA_ARGS__)
#	else
#	define LOGT(...) (void *)0
#	endif
#	if (LOG_LEVEL <= LOG_LEVEL_DEBUG)
#	define LOGD(...) __debug_print(LOG_LEVEL_DEBUG, __func__, __LINE__, __VA_ARGS__)
#	else
#	define LOGD(...) (void *)0
#	endif
#	if (LOG_LEVEL <= LOG_LEVEL_WARNING)
#	define LOGW(...) __debug_print(LOG_LEVEL_WARNING, __func__, __LINE__, __VA_ARGS__)
#	else
#	define LOGW(...) (void *)0
#	endif
#	if (LOG_LEVEL <= LOG_LEVEL_ERROR)
#	define LOGE(...) __debug_print(LOG_LEVEL_ERROR, __func__, __LINE__, __VA_ARGS__)
#	else
#	define LOGE(...) (void *)0
#	endif
#else
#	define LOGV(...) (void *)0
#	define LOGT(...) (void *)0
#	define LOGD(...) (void *)0
#	define LOGW(...) (void *)0
#	define LOGE(...) (void *)0
#endif


void __debug_log_print(int level, const char * fn_name, int line, const char * tag, char const * fmt, ...);
void __debug_print(int level, const char *fn_name, int line, const char * fmt, ...);
double get_time_ms(struct timeval *tstart, struct timeval *tstop);

#endif // DBGTOOLS_H
