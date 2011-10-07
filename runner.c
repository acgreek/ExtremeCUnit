#include "ut_config.h"
#include "unittest.h"


int run_tests(ut_configuration_t * configp) {
	int result=0;
	
	result += test_func_test_1(); 
	result += test_func_test_2(); 
	return result;
}
