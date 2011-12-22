#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "ut_config.h"
#define UNIT_TEST
#include "suite_and_test_list_wrapper.h"
#undef UNIT_TEST


char * getUnitTestListAsString(ut_configuration_t *configp,ListNode_t *test_suites_list_headp);

int run_tests(ut_configuration_t * configp, ListNode_t *test_suites_list_headp);


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

test_suite_element_t * create_test_suite_element() {
	test_suite_element_t * e = malloc(sizeof(test_suite_element_t));
	e->suitep=NULL;	
	ListInitialize(&e->test_list_head);
	return e;
}
test_element_t * create_test_element() {
	test_element_t * e = malloc(sizeof(test_element_t));
	return e;
}
static test_suite_t g_default_test_suite = {
	"",
	"default",
	"",
	0,
	NULL,
	NULL
};
static void addDefaultSuite(ListNode_t *test_suites_list_head) {
	test_suite_element_t * e = create_test_suite_element();
	ListAddEnd(test_suites_list_head, &e->link);
	e->suitep = &g_default_test_suite;

}


int main (int argc, char * argv[]) {
	ut_configuration_t config = UT_CONFIGURATION_DEFAULT;
	int result = 0;

	ListNode_t test_suites_list_head;
	ListInitialize(&test_suites_list_head);
	addDefaultSuite(&test_suites_list_head);

	readCmdConfig(argc, argv, &config);

	config.dynlibraryp = dlopen(argv[0], RTLD_NOW);	
	if (NULL == config.dynlibraryp ) {
		printf("%s\n", dlerror());
		return(-1);
	}
	getUnitTestListAsString(&config, &test_suites_list_head);

	result = run_tests(&config, &test_suites_list_head);
	if (config.verbose) printf("final results: %s\n",0 == result ? "SUCCESS": "FAILED"); 
	dlclose(config.dynlibraryp);
	config.dynlibraryp=NULL;

	return result;
}
