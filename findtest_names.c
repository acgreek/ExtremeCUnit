#include "ut_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#define UNIT_TEST
#include "suite_and_test_list_wrapper.h"
#undef UNIT_TEST
#define COMMAND "nm %s |grep UnitTest_a_test_to_run_|cut -d' ' -f 3|grep -E \"^UnitTest_a_test_to_run_\""
void chomp(char *str) {
	while ('\0' != *str ) {
		if (*str == '\n')
			*str='\0';
		str++;
	}
}

void loadTestStruct(int i,ListNode_t *test_suites_list_headp, char *line,ut_configuration_t *configp) {
	chomp(line);
	test_results_t *trp;
	trp = dlsym(configp->dynlibraryp, line);
	test_element_t * e = create_test_element();
	memcpy(&e->test, trp, sizeof(test_results_t));
	test_suite_element_t *sp = NODE_TO_ENTRY(test_suite_element_t,link,test_suites_list_headp->nextp);
	ListAddEnd(&sp->test_list_head, &e->link);
}
char * getUnitTestListAsString(ut_configuration_t *configp,ListNode_t *test_suites_list_headp) {
	char command_line[1024];

	snprintf(command_line, 1023, COMMAND, configp->program_name);
	FILE * fd = popen (command_line, "r"); 
	int i=0;
	while (!feof(fd)) {
		char line[300];
		if (0 != fgets(line,299, fd)) {
			loadTestStruct(i, test_suites_list_headp, line,configp);
		}
		i++;

	}
	pclose(fd);
	return "";
}
