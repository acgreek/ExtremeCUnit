#ifndef UT_CONFIG_H
#define UT_CONFIG_H
#include <stdlib.h>
#include <stdio.h>
typedef struct _ut_configuration_t {
	char * program_name;
	FILE *output_fd;
	void  *dynlibraryp;
	int verbose;
	int run_in_debugger;
	char * only_test;
	char rerun_in_debugger;
	char run_perf_tests;
	char stop_after_one_failed_test;
	char run_forked; 
	char * run_only_test;
} ut_configuration_t;
#define UT_CONFIGURATION_DEFAULT {argv[0], stdout,NULL,0,0,NULL,0,0,0}
#endif
