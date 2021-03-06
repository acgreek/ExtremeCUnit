// vim: ts=2 noet spell foldmethod=syntax :
#include "ut_config.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

#include "util.h"
#define UNIT_TEST
#include "suite_and_test_list_wrapper.h"
#undef UNIT_TEST

static test_results_t * g_running_test = NULL;

typedef struct _execute_context_t{
	ut_configuration_t * configp;
	test_suite_t *suitep;
	test_suite_element_t *sp;
	int result;
	void *ptr;
	void (*cleanExit)(void *);
}execute_context_t;
char * create_tempfile(char * filename,const  char * suite_name, const  char * test_name){
	int ofid= mkstemp(filename);
	FILE * fid = fdopen (ofid, "w");

	if (NULL == fid){
		perror("opening file:");
		exit(-1);
	}
	fprintf(fid, "break %s%s%s%s\n","test_suite_", suite_name, "_func_", test_name);
	fprintf(fid, "c\n");
	fclose(fid);
	return filename;
}
void segv_handler(UNUSED int sig) {
	fprintf(stderr, "%s:%d:0 segv in  test %s\n",g_running_test->filename, g_running_test->line,g_running_test->test_name);
	exit(-1);

}
int run_test_in_child_stack_monitor(test_results_t *testp, UNUSED void * ptr){
	g_running_test= testp;
	signal(SIGSEGV, segv_handler);
	int results = 0 == testp->func(ptr) ? 0 : 1;
	return results;
}
static void call_std_leaky_func() {
	time_t now = time(NULL);
	char foo[100];
	snprintf(foo, sizeof(foo)-1, "foo %s\n", ctime(&now));

}
//Yuh, globals need to remove
static test_results_t *g_testp;
static struct mallinfo start_mem, end_mem;
static void test_mem_atexit() {
	end_mem = mallinfo();
	if (start_mem.uordblks != end_mem.uordblks) {
		fprintf(stderr, "%s:%d:0 possible memory leak in test '%s': %u bytes unaccounted for\n",g_testp->filename, g_testp->line,g_testp->test_name, end_mem.uordblks- start_mem.uordblks);
		_exit(-1);
	}
}

int run_test_in_child_memcheck_and_stack_monitor(execute_context_t * ecp, test_results_t *testp){
	call_std_leaky_func();
	start_mem= mallinfo();
	g_testp = testp;
	if (ecp->configp->enable_memory_test)
			atexit(test_mem_atexit); // this will be called last on linux because these are called in reverse order of registration, and this is the first registration
	void *ptr=NULL;
	int results=0;
	if (NULL!= ecp->sp->setup.func)
		ptr=ecp->sp->setup.func(NULL);
	results =  run_test_in_child_stack_monitor(testp, ptr);
	if (NULL!= ecp->sp->destroy.func)
		ecp->sp->destroy.func(ptr);

	return results;
}
int run_test_forked_h1(execute_context_t * ecp, test_results_t *testp, int seconds_to_sleep){
	pid_t child_pid;
	if (0 == (child_pid = fork()) ) {
		int results;
		sleep(seconds_to_sleep);
		results =  run_test_in_child_memcheck_and_stack_monitor(ecp, testp);
		ecp->cleanExit(ecp->ptr);
		exit(results);
	}
	return child_pid;
}
int run_test_forked(execute_context_t * ecp, test_results_t *testp){
	pid_t child_pid = run_test_forked_h1(ecp, testp,0);
	alarm(10);
	int status=-1;
	int wait_status = waitpid(child_pid, &status,0);
	alarm(0);
	while (wait_status == EINTR) {
		fprintf(stderr, "%s:%d:0 test '%s' timed out \n",testp->filename, testp->line,testp->test_name);
		kill(child_pid, SIGTERM);
		alarm(10);
		wait_status = waitpid(child_pid, &status,0);
		alarm(0);
		if (wait_status == EINTR) {
			fprintf(stderr, "%s:%d:0 test '%s': even SIGTERM timed out his is bad\n",testp->filename, testp->line,testp->test_name);
			kill(child_pid, SIGKILL);
			wait_status = waitpid(child_pid, &status,0);
		}
	}
	if (!WIFEXITED(status)) {
		fprintf(stderr, "%s:%d:0 test '%s': test terminated unexpectedly\n",testp->filename, testp->line,testp->test_name);
	}

	return (status != 0) ? 1 : 0;
}
int run_test_forked_in_gdb(execute_context_t * ecp, test_results_t *testp){
	int status;
	char buffer[100] = "unittest_XXXXXX";
	char * tempfile = create_tempfile(buffer,testp->suite_name, testp->test_name);
	pid_t child_pid = run_test_forked_h1(ecp, testp,1);
	char bufferp[2000];

	sprintf(bufferp, "%s -x %s -q -p %u",getenvd("GDB", "gdb"),tempfile,  child_pid);
	system (bufferp);
	waitpid(child_pid, &status,0);
	unlink(buffer);
	return status;
}
int run_test(execute_context_t * ecp, test_results_t *testp) {
	int cur_results = 0;

	if (NULL != ecp->configp->only_test) {
		if (0 == (strncmp(ecp->configp->only_test, testp->test_name, strlen(ecp->configp->only_test)+1))) {
			if (ecp->configp->run_in_debugger)
				cur_results = 0 == run_test_forked_in_gdb(ecp,testp) ? 0 :1;
			else  {
				cur_results = 0 == run_test_forked(ecp,testp) ? 0 :1;
			}

		}
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
	if (UTTT_PERFORMANCE == sp->test.test_type && ecp->configp->run_perf_tests == 0) {
		if (ecp->configp->verbose) {
			test_results_t *testp = &sp->test;
			fprintf(ecp->configp->output_fd, "skipping performance test %s in file %s at line %d (to run this test add -p to command line\n", testp->test_name,testp->filename, testp->line);
		}
		return ;
	}
	int  current_failure=0;
	ecp->result +=current_failure=  run_test(ecp, &sp->test) ;
	if (current_failure > 0 && ecp->configp->stop_after_one_failed_test)
		exit(-1);
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
static void sigalarm(UNUSED int sig) {
}
int run_tests(ut_configuration_t * configp,  ListNode_t *test_suites_list_headp, void *ptr, void cleanExit(void *)) {
	execute_context_t ec;
	ec.configp = configp;
	ec.result = 0;
	ec.ptr = ptr;
	ec.cleanExit = cleanExit;
 	signal(SIGALRM,sigalarm );
	ListApplyAll (test_suites_list_headp, executeSuite, &ec);
	return ec.result;
}
