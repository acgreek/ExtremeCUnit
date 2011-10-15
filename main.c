#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "ut_config.h"
#define UNIT_TEST
#include "unittest.h"
#undef UNIT_TEST


char * getUnitTestListAsString(ut_configuration_t *configp,test_results_t *testsp);

int run_tests(ut_configuration_t * configp, test_results_t *testsp);

static void readCmdConfig(int argc, char * argv[], ut_configuration_t *configp) {
	int option;
	while (-1 != (option =getopt(argc,argv, "g:"))) {
		switch (option) {
			case 'g':
				configp->gdb_test = optarg;		
				break;
		}
	}

}
int main (int argc, char * argv[]) {
	ut_configuration_t config = UT_CONFIGURATION_DEFAULT;
	int result = 0;
	test_results_t tests[50] ;

	readCmdConfig(argc, argv, &config);

	config.dynlibraryp = dlopen(argv[0], RTLD_NOW);	
	if (NULL == config.dynlibraryp ) {
		printf("%s\n", dlerror());
		return(-1);
	}
	getUnitTestListAsString(&config, tests);

	result = run_tests(&config, tests);
	if (stdout == config.output_fd) printf("final results: %s\n",0 == result ? "SUCCESS": "FAILED"); 
	dlclose(config.dynlibraryp);
	config.dynlibraryp=NULL;

	return result;
}
