#ifndef UNITTEST_H
#define UNITTEST_H
#ifdef UNIT_TEST

//int test_wrapper (char * testname, char * filename, int line_number) ;
typedef struct _test_results_t {
	char * test_name;
	char * filename;
	int line;
	int (*func)();
}test_results_t;

typedef struct _test_suite_results_t {
	char * suite_name;
	int number_of_tests;
	test_results_t * test_resultsp;
}test_suite_results_t;

#define TEST(NAME)  int test_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_ ## NAME = {#NAME, __FILE__, __LINE__, test_func_ ## NAME};\
	int test_func_ ## NAME() 

#endif 
#endif
