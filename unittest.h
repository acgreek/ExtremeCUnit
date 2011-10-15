#ifndef UNITTEST_H
#define UNITTEST_H
#ifdef UNIT_TEST
#define UNITTEST_CURRENT_SUITE "global"

#define TEST_MAGIC_STRING "This is a unit test"

typedef struct _test_config_t  {
	int verbose;
} test_config_t ;

//int test_wrapper (char * testname, char * filename, int line_number) ;
typedef struct _test_results_t {
	char * magic;
	char * suite_name;
	char * test_name;
	char * filename;
	int line;
	int (*func)();
}test_results_t;


#define TEST(NAME)  int test_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_ ## NAME = {TEST_MAGIC_STRING, UNITTEST_CURRENT_SUITE, #NAME, __FILE__, __LINE__, test_func_ ## NAME};\
	int test_func_ ## NAME() 
int ut_assertStatement(int result, char *statement, char * filename, int line);
#define Assert(STATEM) if (ut_assertStatement(STATEM, #STATEM, __FILE__, __LINE__)){return -1;}

#endif 
#endif
