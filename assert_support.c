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
#include <string.h>

static int ut_assertStatementEqStr_nulls(const char *real, const char *expected, const char *statement, char * filename, int line) {
	if ( (real == expected)) {
		return 0;
	}
	fprintf(stderr, "%s:%d:0 statement failed: %s: expected '%s' but was '%s'\n", filename, line, 
			statement, NULL==expected? "NULL": expected, NULL == real? "NULL": real);
	return 1;
}



static void showFirstCharacterDifference(const char *expected, const char *real) {
	int i=0;
	while (*expected == *real) {
		i++; expected++; real++;
	}
	fprintf(stderr, "first difference at offset %d, char is '%c' expected '%c'", i, *real, *expected);
	
}	
int ut_assertStatementEqStr(const char *real, const char *expected, const char *statement, char * filename, int line) {

	if (NULL == real || NULL == expected)  {

		return  ut_assertStatementEqStr_nulls(real, expected, statement, filename,line);
	}
	int result = 0 ==strcmp(real,expected); 
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %s: expected '%s' but was '%s': ", filename, line, 
				statement, expected, real);
		showFirstCharacterDifference(expected,real);
		fprintf(stderr, "\n");

	}
	return 1== result? 0: 1;
}
