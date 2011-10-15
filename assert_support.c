#include <stdio.h>
#include <stdlib.h>

int ut_assertStatement(int result, char *statement, char * filename, int line) {

	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %s\n", filename, line, statement);
	}
	return 1== result? 0: 1;
}
