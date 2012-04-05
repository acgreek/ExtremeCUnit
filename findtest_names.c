#include "ut_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#define UNIT_TEST
#include "suite_and_test_list_wrapper.h"
#undef UNIT_TEST
#define COMMAND "nm %s |grep UnitTest_a_test_to_run_|cut -d' ' -f 3|grep -E \"^UnitTest_a_test_to_run_\""
#define SETUP "nm %s |grep UnitTest_a_unit_test_suite_setup_func_|cut -d' ' -f 3|grep -E \"^UnitTest_a_unit_test_suite_setup_func_\""
#define DESTORY "nm %s |grep UnitTest_a_unit_test_suite_destroy_func_|cut -d' ' -f 3|grep -E \"^UnitTest_a_unit_test_suite_destroy_\""
void chomp(char *str) {
	while ('\0' != *str ) {
		if (*str == '\n')
			*str='\0';
		str++;
	}
}
static int isSuite(ListNode_t * nodep, void * datap) {
	char * suite_name = (char *) datap;
	test_suite_element_t *sp = NODE_TO_ENTRY(test_suite_element_t,link,nodep);
	return 0==strcmp(suite_name, sp->suite.suite_name);
}
static void addTestToSuites(test_element_t *e,ListNode_t * test_suites_list_headp) {
	ListNode_t * listp = ListFind(test_suites_list_headp,isSuite, (char *)e->test.suite_name);
	if (NULL == listp) {
		test_suite_element_t *sp = create_test_suite_element( (char *)e->test.suite_name);
		ListAddEnd(test_suites_list_headp, &sp->link);
		listp = &sp->link;
	}
	test_suite_element_t *sp= NODE_TO_ENTRY(test_suite_element_t,link,listp);
	ListAddEnd(&sp->test_list_head, &e->link);
}

void loadTestStruct(ListNode_t *test_suites_list_headp, char *line,ut_configuration_t *configp) {
	test_results_t *trp;
	trp = dlsym(configp->dynlibraryp, line);
	test_element_t * e = create_test_element(trp);
	addTestToSuites(e, test_suites_list_headp);
}
void loadSuiteSetup(ListNode_t *test_suites_list_headp, char *line,ut_configuration_t *configp) {
	test_suite_t *trp;
	trp = dlsym(configp->dynlibraryp, line);
	ListNode_t * listp = ListFind(test_suites_list_headp,isSuite, (char *)trp->suite_name);
	if (NULL == listp) {
		return;
	}
	test_suite_element_t *sp= NODE_TO_ENTRY(test_suite_element_t,link,listp);
	memcpy(&sp->setup, trp, sizeof(test_suite_t));
}
void loadSuiteDestroy(ListNode_t *test_suites_list_headp, char *line,ut_configuration_t *configp) {
	test_suite_t *trp;
	trp = dlsym(configp->dynlibraryp, line);
	ListNode_t * listp = ListFind(test_suites_list_headp,isSuite, (char *)trp->suite_name);
	if (NULL == listp) {
		return;
	}
	test_suite_element_t *sp= NODE_TO_ENTRY(test_suite_element_t,link,listp);
	memcpy(&sp->destroy, trp, sizeof(test_suite_t));

}

void loadTests(ut_configuration_t *configp,ListNode_t *test_suites_list_headp, const char *command, void func(ListNode_t *test_suites_list_headp, char *line,ut_configuration_t *configp)) {
	char command_line[2024];
	snprintf(command_line, sizeof(command_line) -1, command, configp->program_name);
	FILE * fd = popen (command_line, "r"); 
	while (!feof(fd)) {
		char line[300];
		if (0 != fgets(line,299, fd)) {
			chomp(line);
			func( test_suites_list_headp, line,configp);
		}
	}
	pclose(fd);
}
char * getUnitTestListAsString(ut_configuration_t *configp,ListNode_t *test_suites_list_headp) {
	loadTests(configp, test_suites_list_headp, COMMAND,loadTestStruct);
	loadTests(configp, test_suites_list_headp, SETUP,loadSuiteSetup);
	loadTests(configp, test_suites_list_headp, DESTORY,loadSuiteDestroy);
	return "";
}
