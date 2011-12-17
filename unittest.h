#ifndef UNITTEST_H
#define UNITTEST_H
#ifdef UNIT_TEST
#define UNITTEST_GLOBAL_SUITE "global"

#define TEST_MAGIC_STRING "This is a unit test"

typedef struct _test_config_t  {
	int verbose;
} test_config_t ;

//int test_wrapper (char * testname, char * filename, int line_number) ;
typedef struct _test_results_t {
	const char * magic;
	const char * suite_name;
	const char * test_name;
	const char * filename;
	int line;
	int (*func)(void);
}test_results_t;

typedef struct _test_suite_t {
	char * magic;
	char * suite_name;
	char * filename;
	int line;
	enum {UTSS_SETUP, UTSS_DESTROY} function_type;
	void * (*func)(void *);
} test_suite_t;

#define SUITE_SETUP(SUITE_NAME)  int test_suite_setup_func_ ## SUITE_NAME();\
	test_suite_t UnitTest_a_unit_test_suite_setup_func_ ## SUITE_NAME = {TEST_MAGIC_STRING, #SUITE_NAME,  __FILE__, __LINE__,UTSS_SETUP,  test_func_ ## SUITE_NAME};\
	int test_func_ ## NAME() 

#define SUITE_DESTROY(SUITE_NAME)  int test_suite_setup_func_ ## SUITE_NAME();\
	test_suite_t UnitTest_a_unit_test_suite_destroyfunc_ ## SUITE_NAME = {TEST_MAGIC_STRING, #SUITE_NAME,  __FILE__, __LINE__,UTSS_DESTROY,  test_func_ ## SUITE_NAME};\
	int test_func_ ## NAME() 

#define TEST(NAME)  int test_func_ ## NAME(void);\
	test_results_t UnitTest_a_test_to_run_ ## NAME = {TEST_MAGIC_STRING, UNITTEST_GLOBAL_SUITE, #NAME, __FILE__, __LINE__, test_func_ ## NAME};\
	int test_func_ ## NAME(void) 

#define SUITE_TEST(SUITE_NAME,NAME)  int test_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_ ## NAME = {TEST_MAGIC_STRING, #SUITE_NAME, #NAME, __FILE__, __LINE__, test_func_ ## NAME};\
	int test_func_ ## NAME() 

#ifdef __cplusplus
extern "C"  {
#endif
int ut_assertStatement(int result,const char *statement,const char * filename, int line);
#ifdef __cplusplus
}
#endif
#define Assert(STATEM) if (ut_assertStatement(STATEM, #STATEM, __FILE__, __LINE__)){return -1;}
#define AssertEqInt(STATEM, expect) if (ut_assertStatementEqInt(STATEM,expect, #STATEM, __FILE__, __LINE__)){return -1;}

#endif 
#endif
