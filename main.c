// vim: set sw=4 ts=4 sts=4 noet tw=78  foldlevel=0 foldmethod=syntax spell:
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "ut_config.h"
#define UNIT_TEST
#include "suite_and_test_list_wrapper.h"
#undef UNIT_TEST

#include <string.h>

char * getUnitTestListAsString(ut_configuration_t *configp,ListNode_t *test_suites_list_headp);
int run_tests(ut_configuration_t * configp, ListNode_t *test_suites_list_headp);

void usage(UNUSED int argc, char * argv[]) {
	printf("Usage: %s [OPTION]...\n\n", argv[0]);
	printf("Debugging:\n");
	printf("   -o TEST_NAME     skip all tests except TEST_NAME\n");
	printf("   -g TEST_NAME     skip all tests except TEST_NAME and start TEST_NAME in debugger\n");
	printf("   -G               if any test fails, rerun it with debugger\n");
	printf("   -l               stop after one failed test\n");
	printf("   -p               run performance tests (performance test don't execute by default)\n");
	printf("Output:\n");
	printf("   -v               verbose, normally only failed test are displayed\n");
	printf("   -M               enabled memory check, it's better to use valgrind because this memory check does not support a suppression yet as valgrind does\n");
}
static void readCmdConfig(int argc, char * argv[], ut_configuration_t *configp) {
	int option;
	while (-1 != (option =getopt(argc,argv, "o:g:Gv1pM"))) {
		switch (option) {
			case 'G':
				configp->rerun_in_debugger = 1;
				break;
			case 'o':
				configp->only_test = optarg;
				break;
			case 'g':
				configp->run_in_debugger = 1;
				configp->only_test = optarg;
				break;
			case '1':
				configp->stop_after_one_failed_test= 1;
				break;

			case 'p':
				configp->run_perf_tests = 1;
				break;
			case 'v':
				configp->verbose = 1;
				break;
			case 'M':
				configp->enable_memory_test= 1;
				break;
			default:
			case 'h':
				usage (argc,argv);
				exit('h' == option ? 0 : -1);
		}
	}

}
test_suite_element_t * create_test_suite_element(char * suite_name) {
	test_suite_element_t * e =  (test_suite_element_t *) malloc(sizeof(test_suite_element_t));
	memset(e, '\0', sizeof(test_suite_element_t));
	e->suite.suite_name = suite_name;
	ListInitialize(&e->test_list_head);
	return e;
}
test_element_t * create_test_element(test_results_t * testp) {
	test_element_t * e =  (test_element_t *)malloc(sizeof(test_element_t));
	memcpy(&e->test, testp, sizeof(test_results_t));
	return e;
}
static void addDefaultSuite(ListNode_t *test_suites_list_head) {
	test_suite_element_t * e = create_test_suite_element(UNITTEST_GLOBAL_SUITE);
	ListAddEnd(test_suites_list_head, &e->link);
}
static void freeTest(ListNode_t * nodep, UNUSED void * datap) {
	ListRemove(nodep);
	test_element_t * e = NODE_TO_ENTRY(test_element_t, link, nodep);
	free(e);
}
static void freeSuite(ListNode_t * nodep, UNUSED void * datap) {
	ListRemove(nodep);
	test_suite_element_t * e = NODE_TO_ENTRY(test_suite_element_t, link, nodep);
	ListApplyAll(&e->test_list_head,freeTest,  NULL);
	free(e);
}

/**
 * Cygwin needs some hackyness
 */
#ifdef __CYGWIN__
int windows_main (int argc, char * argv[])
#else
int main (int argc, char * argv[])
#endif
{
	ut_configuration_t config = UT_CONFIGURATION_DEFAULT;
	int result = 0;

	ListNode_t test_suites_list_head;
	ListInitialize(&test_suites_list_head);
	addDefaultSuite(&test_suites_list_head);

	readCmdConfig(argc, argv, &config);

#ifdef __CYGWIN__
	char buffer[10000];
	snprintf(buffer, sizeof(buffer) -1, "%s.exe",argv[0]);
	argv[0] = buffer;
#endif
	config.dynlibraryp = dlopen(argv[0], RTLD_LAZY) ;

	if (NULL == config.dynlibraryp ) {
		printf("%s\n", dlerror());
		return(-1);
	}
	getUnitTestListAsString(&config, &test_suites_list_head); result = run_tests(&config, &test_suites_list_head);
	if (config.verbose) printf("final results: %s\n",0 == result ? "SUCCESS": "FAILED");
	ListApplyAll(&test_suites_list_head,freeSuite,  NULL);

	dlclose(config.dynlibraryp);
	config.dynlibraryp=NULL;
	return result;
}
