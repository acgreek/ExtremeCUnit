#include "ut_config.h"
int run_tests(ut_configuration_t * configp) {
	int result=0;
	result += test_1(); 
	result += test_2(); 
	return result;
}
