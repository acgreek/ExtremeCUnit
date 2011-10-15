#ifndef UT_CONFIG_H
#define UT_CONFIG_H
#include <stdlib.h>
#include <stdio.h>
typedef struct _ut_configuration_t {
	char * program_name;
	FILE *output_fd;
	void  *dynlibraryp;
	int verbose;
	char * gdb_test;
} ut_configuration_t;
#define UT_CONFIGURATION_DEFAULT {argv[0], stdout,NULL,1}
#endif
