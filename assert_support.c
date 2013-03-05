// vim: ts=2 noet spell foldlevel=0 foldmethod=syntax :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ut_assertStatement(int result, const char *statement, const char * filename, int line) {
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %s\n", filename, line, statement);
	}
	return 1== result? 0: 1;
}
int ut_assertStatementOptInt(const int real, const int  expect, const int  result, const char *operator, char * filename, int line) {
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %d %s %d\n", filename, line,real, operator, expect);
	}
	return 1== result? 0: 1;
}
int ut_assertStatementOptLong(const long real, const long expect,const int  result, const char *operator, char * filename, int line) {
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %ld %s %ld\n", filename, line,real, operator, expect);
	}
	return 1== result? 0: 1;
}
int ut_assertStatementOptFloat(const float real, const float expect,const int  result, const char *operator, char * filename, int line) {
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %g %s %g\n", filename, line,real, operator, expect);
	}
	return 1== result? 0: 1;
}
int ut_assertStatementOptDouble(const double real, const double expect,const int  result, const char *operator, char * filename, int line) {
	if (!result) {
		fprintf(stderr, "%s:%d:0 statement failed: %g %s %g\n", filename, line,real, operator, expect);
	}
	return 1== result? 0: 1;
}
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
