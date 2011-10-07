#include "ut_config.h"
#include <stdio.h>
#define COMMAND "nm %s |grep UnitTest_a_test_to_run_|cut -d' ' -f 3"
void chomp(char *str) {
	while ('\0' != *str ) {
		if (*str == '\n')
			*str='\0';
		str++;
	}
}

char * getUnitTestListAsString(ut_configuration_t *configp) {
	char command_line[1024];
	snprintf(command_line, 1023, COMMAND, configp->program_name);
	FILE * fd = popen (command_line, "r"); 
	while (!feof(fd)) {
		char line[300];
		if (0 != fgets(line,299, fd)) {
			chomp(line);
			printf("found test: %s\n", line);
		}

	}
	return "";
}
