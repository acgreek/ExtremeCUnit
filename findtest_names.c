#include "ut_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#define UNIT_TEST
#include "unittest.h"
#undef UNIT_TEST
#define COMMAND "nm %s |grep UnitTest_a_test_to_run_|cut -d' ' -f 3"
void chomp(char *str) {
	while ('\0' != *str ) {
		if (*str == '\n')
			*str='\0';
		str++;
	}
}

void loadTestStruct(int i, test_results_t *testsp, char *line,ut_configuration_t *configp) {
	chomp(line);
	test_results_t *trp;
	trp = dlsym(configp->dynlibraryp, line);
	memcpy(&testsp[i], trp, sizeof(test_results_t));
	memset(&testsp[i+1], '\0', sizeof(test_results_t));

}
char * getUnitTestListAsString(ut_configuration_t *configp,test_results_t *testsp) {
	char command_line[1024];

	snprintf(command_line, 1023, COMMAND, configp->program_name);
	FILE * fd = popen (command_line, "r"); 
	int i=0;
	while (!feof(fd)) {
		char line[300];
		if (0 != fgets(line,299, fd)) {
			loadTestStruct(i, testsp, line,configp);
		}
		i++;

	}
	pclose(fd);
	return "";
}
