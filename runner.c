#include "ut_config.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>


#include <sys/types.h>
#include <signal.h>

#define UNIT_TEST
#include "unittest.h"
#undef UNIT_TEST

char * create_tempfile(char * filename, char * test_name){
	FILE * fid = fopen (filename, "w");
	if (NULL == fid){
		perror("opening file:");
		exit(-1);
	}
	fprintf(fid, "break %s%s\n","test_func_", test_name);
	fprintf(fid, "c\n");
	fclose(fid);
	return filename;
}




int run_test_in_child(ut_configuration_t * configp, test_results_t *testp){
	int results = 0 == testp->func() ? 0 : 1;
	return results;
}
int run_test_forked_h1(ut_configuration_t * configp, test_results_t *testp, int seconds_to_sleep){
	pid_t child_pid;
	if (0 == (child_pid = fork()) ) {
		int results;
		sleep(seconds_to_sleep);
		results =  run_test_in_child(configp, testp);
		exit(results);
	}
	return child_pid;
}

int run_test_forked(ut_configuration_t * configp, test_results_t *testp){
	int status;
	pid_t child_pid = run_test_forked_h1(configp, testp,0);
	waitpid(child_pid, &status,0);
	if (status != 0) 
		return 1;
	return 0;
}
int run_test_forked_in_gdb(ut_configuration_t * configp, test_results_t *testp){
	char buffer[100] = "unittest_XXXXXX";
	char * tempfile = create_tempfile(buffer, testp->test_name);
	pid_t child_pid = run_test_forked_h1(configp, testp,1);
	char bufferp[2000];
	sprintf(bufferp, "gdb -x %s -q -p %u",tempfile,  child_pid);
	system (bufferp);
}

int run_test(ut_configuration_t * configp, test_results_t *testp) {
	int cur_results = 0;

	if (NULL != configp->gdb_test) {
		if (0 == strncmp(configp->gdb_test, testp->test_name, strlen(configp->gdb_test)))
			cur_results = 0 == run_test_forked_in_gdb(configp,testp) ? 0 :1; 

		return cur_results;
	}
	else 
		cur_results = 0 == run_test_forked(configp,testp) ? 0 :1; 
	if (configp->verbose) {
		fprintf(configp->output_fd, "%s : %s\n", testp->test_name, 0==cur_results?"SUCCESS": "FAILED");
		fflush(configp->output_fd);
	}
	return cur_results;
}


int run_tests(ut_configuration_t * configp, test_results_t *testsp) {
	int result=0;
	int i=0;
	while (NULL != testsp[i].test_name) { 
		result += run_test(configp, &testsp[i]) ;
		i++;
	}
	return result;
}
