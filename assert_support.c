#include <stdio.h>
#include <stdlib.h>

int ut_assertStatement(int result, const char *statement, const char * filename, int line) {

	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %s\n", filename, line, statement);
	}
	return 1== result? 0: 1;
}
int ut_assertStatementEqInt(int real, const int expect, const char *statement, char * filename, int line) {
	int result =real == expect; 
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %s: expected %d but was %d\n", filename, line, statement, expect, real);
	}
	return 1== result? 0: 1;
}
