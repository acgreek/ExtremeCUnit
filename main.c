#include <stdio.h>
#include <dlfcn.h>
#include "ut_config.h"
#define UNIT_TEST
#include "unittest.h"
#undef UNIT_TEST

int ut_assertStatement(int result, char *statement, char * filename, int line) {
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %s\n", filename, line, statement);
	}
	return result;
}

char * getUnitTestListAsString(ut_configuration_t *configp,test_results_t *testsp);
int run_tests(ut_configuration_t * configp, test_results_t *testsp);
int main (int argc, char * argv[]) {
	ut_configuration_t config = UT_CONFIGURATION_DEFAULT;
	int result = 0;
	test_results_t tests[50] ;
	config.dynlibraryp = dlopen(argv[0], RTLD_NOW);	
	if (NULL == config.dynlibraryp ) {
		printf("%s\n", dlerror());
		return(-1);
	}
	getUnitTestListAsString(&config, tests);

	result = run_tests(&config, tests);
	if (-1 != config.output_fd) printf("final results: %s\n",0 == result ? "SUCCESS": "FAILED"); 

	dlclose(config.dynlibraryp);
	return result;
}
