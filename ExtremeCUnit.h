#ifndef UNITTEST_H
#define UNITTEST_H
#ifdef UNIT_TEST
#define UNITTEST_GLOBAL_SUITE "global"

#define TEST_MAGIC_STRING "This is a unit test"

typedef struct _test_config_t  {
	int verbose;
} test_config_t ;

typedef enum {UTTT_UNIT, UTTT_PERFORMANCE} ut_test_type_t;
//int test_wrapper (char * testname, char * filename, int line_number) ;
typedef struct _test_results_t {
	const char * magic;
	const char * suite_name;
	const char * test_name;
	const char * filename;
	int line;
	int (*func)(void *);
	ut_test_type_t test_type;
}test_results_t;

typedef enum {UTSS_SETUP,UTSS_DESTROY} ut_suite_type_t;
typedef struct _test_suite_t {
	const char * magic;
	const char * suite_name;
	const char * filename;
	int line;
	ut_suite_type_t type;
	void * (*func)(void *);
} test_suite_t;

#define SUITE_SETUP(SUITE_NAME)  void * test_suite_setup_func_ ## SUITE_NAME();\
	test_suite_t UnitTest_a_unit_test_suite_setup_func_ ## SUITE_NAME = {TEST_MAGIC_STRING, #SUITE_NAME,  __FILE__, __LINE__,UTSS_SETUP, (void *(*)(void*)) test_suite_setup_func_ ## SUITE_NAME};\
	void * test_suite_setup_func_ ## SUITE_NAME() 

#define SUITE_DESTROY(SUITE_NAME)  void *test_suite_destroy_func_ ## SUITE_NAME();\
	test_suite_t UnitTest_a_unit_test_suite_destroy_func_ ## SUITE_NAME = {TEST_MAGIC_STRING, #SUITE_NAME,  __FILE__, __LINE__,UTSS_DESTROY,  (void *(*)(void*))  test_suite_destroy_func_ ## SUITE_NAME};\
	void * test_suite_destroy_func_ ## SUITE_NAME() 

#define SUITE_DESTROYV(SUITE_NAME,VAR)  void *test_suite_destroy_func_ ## SUITE_NAME(void *);\
	test_suite_t UnitTest_a_unit_test_suite_destroy_func_ ## SUITE_NAME = {TEST_MAGIC_STRING, #SUITE_NAME,  __FILE__, __LINE__,UTSS_DESTROY,  test_suite_destroy_func_ ## SUITE_NAME};\
	void * test_suite_destroy_func_ ## SUITE_NAME(void *VAR) 

#define TEST(NAME)  int test_suite_global_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_global_ ## NAME = {TEST_MAGIC_STRING, UNITTEST_GLOBAL_SUITE, #NAME, __FILE__, __LINE__,(int (*)(void*)) test_suite_global_func_ ## NAME, UTTT_UNIT};\
	int test_suite_global_func_ ## NAME() 

#define TEST_T(NAME, TEST_TYPE)  int test_suite_global_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_global_ ## NAME = {TEST_MAGIC_STRING, UNITTEST_GLOBAL_SUITE, #NAME, __FILE__, __LINE__,(int (*)(void*)) test_suite_global_func_ ## NAME, TEST_TYPE};\
	int test_suite_global_func_ ## NAME() 

#define TEST_V(NAME,VAR)  int test_suite_global_func_ ## NAME(void*);\
	test_results_t UnitTest_a_test_to_run_global_ ## NAME = {TEST_MAGIC_STRING, UNITTEST_GLOBAL_SUITE, #NAME, __FILE__, __LINE__, test_suite_global_func_ ## NAME, UTTT_UNIT};\
	int test_suite_global_func_ ## NAME(void* ##VAR) 

#define TEST_VT(NAME,VAR,TEST_TYPE)  int test_suite_global_func_ ## NAME(void*);\
	test_results_t UnitTest_a_test_to_run_global_ ## NAME = {TEST_MAGIC_STRING, UNITTEST_GLOBAL_SUITE, #NAME, __FILE__, __LINE__, test_func_ ## NAME, TEST_TYPE};\
	int test_suite_global_func_ ## NAME(void* ##VAR) 

#define SUITE_TEST(SUITE_NAME,NAME)  int test_suite_## SUITE_NAME ##_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_## SUITE_NAME ##_ ## NAME = {TEST_MAGIC_STRING, #SUITE_NAME, #NAME, __FILE__, __LINE__, test_suite_## SUITE_NAME ##_func_ ## NAME, UTTT_UNIT};\
	int test_suite_## SUITE_NAME ##_func_ ## NAME() 

#define SUITE_TEST_T(SUITE_NAME,NAME, TEST_TYPE)  int test_suite_## SUITE_NAME ##_func_ ## NAME();\
	test_results_t UnitTest_a_test_to_run_## SUITE_NAME ##_ ## NAME = {TEST_MAGIC_STRING, #SUITE_NAME, #NAME, __FILE__, __LINE__, test_suite_## SUITE_NAME ##_func_ ## NAME, TEST_TYPE};\
	int test_suite_## SUITE_NAME ##_func_ ## NAME() 

#define SUITE_TEST_V(SUITE_NAME,NAME,VAR)  int test_suite_## SUITE_NAME ## _func_ ## NAME(void * );\
	test_results_t UnitTest_a_test_to_run_## SUITE_NAME ##_ ## NAME = {TEST_MAGIC_STRING, #SUITE_NAME, #NAME, __FILE__, __LINE__, test_suite_ ## SUITE_NAME ## _func_ ## NAME, UTTT_UNIT};\
	int test_suite_## SUITE_NAME ##_func_ ## NAME(void * VAR) 

#define SUITE_TEST_VT(SUITE_NAME,NAME,VAR, TEST_TYPE)  int test_suite_## SUITE_NAME ## _func_ ## NAME(void * );\
	test_results_t UnitTest_a_test_to_run_## SUITE_NAME ##_ ## NAME = {TEST_MAGIC_STRING, #SUITE_NAME, #NAME, __FILE__, __LINE__, test_suite_ ## SUITE_NAME ## _func_ ## NAME, TEST_TYPE};\
	int test_suite_## SUITE_NAME ##_func_ ## NAME(void * VAR) 


#ifdef __cplusplus
extern "C"  {
#endif
int ut_assertStatement(int result,const char *statement,const char * filename, int line);
int ut_assertStatementOptInt(int real, int exected,int result, const char *statement,const char * filename, int line);
int ut_assertStatementEqStr(const char * real,const char * expected ,const char *statement,const char * filename, int line);
int ut_assertStatementOptLong(const long real, const long expect,const int  result, const char *oper, char * filename, int line);
int ut_assertStatementOptFloat(const float real, const float expect,const int  result, const char *oper, char * filename, int line);
int ut_assertStatementOptDouble(const double real, const double expect,const int  result, const char *oper, char * filename, int line);
#ifdef __cplusplus
}
#endif
#define Assert(STATEM) if (ut_assertStatement(STATEM, #STATEM, __FILE__, __LINE__)){return -1;}
#define AssertEqInt(STATEM, expect) {int ExtremeCUnitInt= STATEM ;if (ut_assertStatementOptInt(ExtremeCUnitInt,expect,ExtremeCUnitInt == expect,  "==", __FILE__, __LINE__)){return -1;} }
#define AssertOptInt(STATEM,OPT, expect) {int ExtremeCUnitInt= STATEM ; if (ut_assertStatementOptInt(ExtremeCUnitInt ,expect,ExtremeCUnitInt OPT expect,#OPT , __FILE__, __LINE__)){return -1;} }
#define AssertOptFloat(STATEM,OPT, expect) {int ExtremeCUnitFloat=STATEM; if (ut_assertStatementOptFloat(ExtremeCUnitFloat,expect,ExtremeCUnitFloat OPT expect,#OPT , __FILE__, __LINE__)){return -1;} }
#define AssertOptDouble(STATEM,OPT, expect) {double ExtremeCUnitDouble=STATEM; if (ut_assertStatementOptDouble(ExtremeCUnitDouble,expect,ExtremeCUnitDouble OPT expect,#OPT , __FILE__, __LINE__)){return -1;} } 
#define AssertEqStr(STATEM, expect) if (ut_assertStatementEqStr(STATEM,expect, #STATEM, __FILE__, __LINE__)){return -1;}

#endif 
#endif
