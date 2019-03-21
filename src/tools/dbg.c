#include "dbg.h"


#define DBG_STR_BUF_SIZE	2048

#ifdef COLORED_TERM
#define __DEBUG_LOG_HEADER	F_WHITE "[" F_CYAN "%s" F_WHITE ":" F_CYAN "%d" F_WHITE "][" F_YELLOW "%s" F_WHITE "]"
#define __DEBUG_HEADER	F_WHITE "[" F_CYAN "%s" F_WHITE ":" F_CYAN "%d" F_WHITE "]"
#define LOG_V_COLOR			F_WHITE
#define LOG_I_COLOR			C_BOLD F_GREEN
#define LOG_T_COLOR			F_YELLOW
#define LOG_D_COLOR			F_CYAN
#define LOG_W_COLOR			C_BOLD F_YELLOW
#define LOG_E_COLOR			C_BOLD F_RED
#else
#define __DEBUG_LOG_HEADER	 "[%s:%d][%s]"
#define __DEBUG_HEADER	 "[%s:%d]"
#endif

#ifdef COLORED_TERM
static const char __debug_log_v_format[] = __DEBUG_LOG_HEADER LOG_V_COLOR " MSG: %s" C_ENDLINE;
static const char __debug_log_i_format[] = __DEBUG_LOG_HEADER LOG_I_COLOR " INF: %s" C_ENDLINE;
static const char __debug_log_t_format[] = __DEBUG_LOG_HEADER LOG_T_COLOR " THR: %s" C_ENDLINE;
static const char __debug_log_d_format[] = __DEBUG_LOG_HEADER LOG_D_COLOR " DBG: %s" C_ENDLINE;
static const char __debug_log_w_format[] = __DEBUG_LOG_HEADER LOG_W_COLOR " WRN: %s" C_ENDLINE;
static const char __debug_log_e_format[] = __DEBUG_LOG_HEADER LOG_E_COLOR " ERR: %s" C_ENDLINE;

static const char __debug_v_format[] = __DEBUG_HEADER LOG_V_COLOR " MSG: %s" C_ENDLINE ;
static const char __debug_i_format[] = __DEBUG_HEADER LOG_I_COLOR " INF: %s" C_ENDLINE ;
static const char __debug_t_format[] = __DEBUG_HEADER LOG_T_COLOR " THR: %s" C_ENDLINE ;
static const char __debug_d_format[] = __DEBUG_HEADER LOG_D_COLOR " DBG: %s" C_ENDLINE ;
static const char __debug_w_format[] = __DEBUG_HEADER LOG_W_COLOR " WRN: %s" C_ENDLINE ;
static const char __debug_e_format[] = __DEBUG_HEADER LOG_E_COLOR " ERR: %s" C_ENDLINE ;
#else
static const char __debug_log_v_format[] = __DEBUG_LOG_HEADER " MSG: %s" C_ENDLINE;
static const char __debug_log_t_format[] = __DEBUG_LOG_HEADER " THR: %s" C_ENDLINE;
static const char __debug_log_d_format[] = __DEBUG_LOG_HEADER " DBG: %s" C_ENDLINE;
static const char __debug_log_w_format[] = __DEBUG_LOG_HEADER " WRN: %s" C_ENDLINE;
static const char __debug_log_e_format[] = __DEBUG_LOG_HEADER " ERR: %s" C_ENDLINE;

static const char __debug_v_format[] = __DEBUG_HEADER " MSG: %s" C_ENDLINE ;
static const char __debug_t_format[] = __DEBUG_HEADER " THR: %s" C_ENDLINE ;
static const char __debug_d_format[] = __DEBUG_HEADER " DBG: %s" C_ENDLINE ;
static const char __debug_w_format[] = __DEBUG_HEADER " WRN: %s" C_ENDLINE ;
static const char __debug_e_format[] = __DEBUG_HEADER " ERR: %s" C_ENDLINE ;
#endif

static const char * __debug_log_format[] = {
	__debug_log_v_format,
	__debug_log_i_format,
	__debug_log_t_format,
	__debug_log_d_format,
	__debug_log_w_format,
	__debug_log_e_format,
	NULL
};

static const char * __debug_format[] = {
	__debug_v_format,
	__debug_i_format,
	__debug_t_format,
	__debug_d_format,
	__debug_w_format,
	__debug_e_format,
	NULL
};

static char __debug_str_buffer[DBG_STR_BUF_SIZE];

void __debug_log_print(int level, const char * fn_name, int line, const char * tag, char const * fmt, ...) {
	va_list vaptr;
	va_start(vaptr, fmt);
	vsprintf(__debug_str_buffer, fmt, vaptr);

	printf(__debug_log_format[level], fn_name, line, tag,__debug_str_buffer);

	va_end(vaptr);
}

void __debug_print(int level, const char *fn_name, int line, const char * fmt, ...) {
	va_list vaptr;
	va_start(vaptr, fmt);
	vsprintf(__debug_str_buffer, fmt, vaptr);
	printf(__debug_format[level], fn_name, line, __debug_str_buffer);
	va_end(vaptr);

}

double get_time_ms(struct timeval *tstart, struct timeval *tstop) {
	if (tstart && tstop) {
		return (((double)(tstop->tv_sec - tstart->tv_sec) * (double)1000)
			+ (((double)(tstop->tv_usec - tstart->tv_usec)) / (double) 1000));;
	}
	return INFINITY;
}
