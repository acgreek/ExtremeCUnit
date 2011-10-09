#include "ut_config.h"
#include <stdio.h>
#define UNIT_TEST
#include "unittest.h"
#undef UNIT_TEST


int run_tests(ut_configuration_t * configp, test_results_t *testsp) {
	int result=0;
	int i=0;
	while (NULL != testsp[i].test_name) { 
		int cur_results = 0 == testsp[i].func() ? 0 :1; 
		if (configp->verbose)
			fprintf(configp->output_fd, "%s : %s\n", testsp[i].test_name, 0==cur_results?"SUCCESS": "FAILED");
		result += cur_results;
		i++;
	}
	return result;
}
