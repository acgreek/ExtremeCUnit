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
#include "suite_and_test_list_wrapper.h"
#undef UNIT_TEST

typedef struct _execute_context_t{
	ut_configuration_t * configp;
	test_suite_t *suitep;
	test_suite_element_t *sp;
	int result;
}execute_context_t;

char * create_tempfile(char * filename,const  char * test_name){
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



#include <malloc.h>

static test_results_t * g_running_test = NULL; 
void segv_handler(int sig) {
	sig=sig;
	fprintf(stderr, "%s:%d:0 segv in  test %s\n",g_running_test->filename, g_running_test->line,g_running_test->test_name);
	exit(-1);

}
int run_test_in_child_stack_monitor(execute_context_t * ecp, test_results_t *testp, UNUSED void * ptr){ 
	g_running_test= testp;
	signal(SIGSEGV, segv_handler);
	int results = 0 == testp->func(ptr) ? 0 : 1;
	return results;
}
int run_test_in_child_memcheck_and_stack_monitor(execute_context_t * ecp, test_results_t *testp){
	struct mallinfo start_mem, end_mem;
	start_mem= mallinfo();

	void *ptr;
	int results=0;
	if (NULL!= ecp->sp->setup.func)
		ptr=ecp->sp->setup.func(NULL);
	results =  run_test_in_child_stack_monitor(ecp, testp, ptr);
	if (NULL!= ecp->sp->destroy.func)
		ecp->sp->destroy.func(ptr);
	
	end_mem = mallinfo();
	if (start_mem.uordblks != end_mem.uordblks) {
		fprintf(stderr, "%s:%d:0 possible memory leak in test %s\n",testp->filename, testp->line,testp->test_name);
		return -1;
	}
	return results;
}
int run_test_forked_h1(execute_context_t * ecp, test_results_t *testp, int seconds_to_sleep){
	pid_t child_pid;
	if (0 == (child_pid = fork()) ) {
		int results;
		sleep(seconds_to_sleep);
		results =  run_test_in_child_memcheck_and_stack_monitor(ecp, testp);
		exit(results);
	}
	return child_pid;
}

int run_test_forked(execute_context_t * ecp, test_results_t *testp){
	int status;
	pid_t child_pid = run_test_forked_h1(ecp, testp,0);
	waitpid(child_pid, &status,0);
	if (status != 0) 
		return 1;
	return 0;
}
int run_test_forked_in_gdb(execute_context_t * ecp, test_results_t *testp){
	int status;
	char buffer[100] = "unittest_XXXXXX";
	char * tempfile = create_tempfile(buffer, testp->test_name);
	pid_t child_pid = run_test_forked_h1(ecp, testp,1);
	char bufferp[2000];
	sprintf(bufferp, "gdb -x %s -q -p %u",tempfile,  child_pid);
	system (bufferp);
	waitpid(child_pid, &status,0);
	return status;
}

int run_test(execute_context_t * ecp, test_results_t *testp) {
	int cur_results = 0;

	if (NULL != ecp->configp->gdb_test) {
		if (0 == strncmp(ecp->configp->gdb_test, testp->test_name, strlen(ecp->configp->gdb_test)))
			cur_results = 0 == run_test_forked_in_gdb(ecp,testp) ? 0 :1; 

		return cur_results;
	}
	cur_results = 0 == run_test_forked(ecp,testp) ? 0 :1; 
	if (ecp->configp->verbose) {
		fprintf(ecp->configp->output_fd, "%s : %s\n", testp->test_name, 0==cur_results?"SUCCESS": "FAILED");
		fflush(ecp->configp->output_fd);
	}
	if (0 != cur_results && ecp->configp->rerun_in_debugger) {
		cur_results = 0 == run_test_forked_in_gdb(ecp,testp) ? 0 :1; 
	}

	return cur_results;
}

void executeTest(ListNode_t * nodep, void * datap) {
	execute_context_t * ecp = (execute_context_t *) datap;
	test_element_t *sp = NODE_TO_ENTRY(test_element_t,link,nodep);
	ecp->result += run_test(ecp, &sp->test) ;
}
void executeSuite(ListNode_t * nodep, void * datap) {
	execute_context_t * ecp = (execute_context_t *) datap;
	ecp->sp = NODE_TO_ENTRY(test_suite_element_t,link,nodep);
	if (ecp->configp->verbose) {
		fprintf(ecp->configp->output_fd, "IN TEST SUITE: %s\n", ecp->sp->suite.suite_name);
		fflush(ecp->configp->output_fd);
	}
	ListApplyAll (&ecp->sp->test_list_head, executeTest,ecp);

}
int run_tests(ut_configuration_t * configp,  ListNode_t *test_suites_list_headp) {
	execute_context_t ec;
	ec.configp = configp;
	ec.result = 0;
	ListApplyAll (test_suites_list_headp, executeSuite, &ec);
	return ec.result;
}
