#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "ut_config.h"
#define UNIT_TEST
#include "unittest.h"
#undef UNIT_TEST


char * getUnitTestListAsString(ut_configuration_t *configp,test_results_t *testsp);

int run_tests(ut_configuration_t * configp, test_results_t *testsp);


void usage(int argc, char * argv[]) {
	printf("Usage: %s [OPTION]...\n\n", argv[0]);	
	printf("Debugging:\n");	
	printf("   -g TEST_NAME     skip all tests except TEST_NAME and start TEST_NAME in debugger\n");	
	printf("   -G               if any test fails, rerun it with debugger\n");
	printf("Output:\n");	
	printf("   -v               verbose, normally only failed test are displayed\n");	
	
}

static void readCmdConfig(int argc, char * argv[], ut_configuration_t *configp) {
	int option;
	while (-1 != (option =getopt(argc,argv, "g:Gv"))) {
		switch (option) {
			case 'G':
				configp->rerun_in_debugger = 1;
				break;
			case 'g':
				configp->gdb_test = optarg;		
				break;
			case 'v':
				configp->verbose = 1;		
				break;
			default:
			case 'h':
				usage (argc,argv);
				exit('h' == option ? 0 : -1);
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
	if (config.verbose) printf("final results: %s\n",0 == result ? "SUCCESS": "FAILED"); 
	dlclose(config.dynlibraryp);
	config.dynlibraryp=NULL;

	return result;
}
