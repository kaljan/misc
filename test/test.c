#include "str.h"
#include "list.h"

#ifdef RNDSTR_TEST
extern int randstr_test(void);
#elif defined RNDDATA_TEST
extern int rnddata_test(void);
#elif defined LIST_TEST
extern int list_test(void);
#elif defined BUF_TEST
extern int test_buffer(void);
#elif defined LOOP_THREAD_TEST
extern int loop_thread_test(void);
#elif defined NET_TEST
int net_test(int argc, char ** argv);
#else
#	warning "Test type is not defined"
#endif

int main(int argc, char ** argv) {
#ifdef RNDSTR_TEST
	return randstr_test();
#elif defined RNDDATA_TEST
	return rnddata_test();
#elif defined LIST_TEST
	return list_test();
#elif defined BUF_TEST
	return test_buffer();
#elif defined LOOP_THREAD_TEST
	return loop_thread_test();
#elif defined NET_TEST
	return net_test(argc, argv);
#else
	return 0;
#endif
}
