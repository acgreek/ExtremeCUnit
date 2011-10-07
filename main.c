#include <stdio.h>
#include "ut_config.h"

int main (int argc, char * argv[]) {
	ut_configuration_t config = UT_CONFIGURATION_DEFAULT;
	int result = 0;
	
	getUnitTestListAsString(&config);

	run_tests(&config);
	if (-1 != config.output_fd) printf("final results: %s\n",0 == result ? "SUCCESS": "FAILED"); 
	return result;
}
