// vim: set sw=4 ts=4 sts=4 noet tw=78  foldlevel=0 foldmethod=syntax spell:
#define UNIT_TEST
#include "ExtremeCUnit.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdlib.h>
TEST(test_1) {
	Assert(1==1);
	return 0;
}
TEST(test_2) {
	//Assert(1==0);
	return 0;
}
void chomp(char * str);
TEST(chomp_test_remove) {
	char foo[200] = "a\n";
	chomp(foo);
	Assert(0 == strcmp(foo,"a"));
	return 0;
}
TEST(int_test) {
	AssertEqInt(10 , 0xa);
	AssertOptInt(10 , <, 11);
	AssertOptDouble(10.3, <, 11.4);
	AssertOptDouble(10.3, <, 10.4);
	return 0;
}
TEST(int_test_opt_should_not_call_twice) {
	int f=0;
	AssertOptInt(++f, ==, 1);
	AssertOptInt(++f, ==, 2);
	AssertEqInt(++f, 3);
	AssertEqInt(++f, 4);

	float flo=0;
	AssertOptFloat(++flo, ==, 1);
	AssertOptFloat(++flo, ==, 2);
	double dl=0;
	AssertOptFloat(++dl, ==, 1);
	AssertOptFloat(++dl, ==, 2);
	long ll=0;
	AssertOptFloat(++ll, ==, 1);
	AssertOptFloat(++ll, ==, 2);
	return 0;
}
TEST(ctime_should_not_leak) {
	time_t now = time(NULL);
	char foo[100];
	snprintf(foo, sizeof(foo)-1, "foo %s\n", ctime(&now));
	return 0;

}
TEST(abort) {
	//abort();
	return 0;

}
TEST(string_test) {
	AssertEqStr("bla" "bla", "blabla");
	AssertEqStr(NULL, NULL);
	/*
	char *c = (char *) malloc(10);

	free(c);
	int i;
	for (i=0;i<900000; i++)
		c[i]='a';
	AssertEqStr(c, "asdfsdf");
	*/
	return 0;
}
TEST_T(string_perf, UTTT_PERFORMANCE) {
	AssertEqStr("bla" "bla", "blabla");
	AssertEqStr(NULL, NULL);
	return 0 ;
}
char *g_foop = 0;

SUITE_SETUP(suite1) {
	g_foop = strdup("blab");
	return 0;
}
SUITE_TEST(suite1,test1) {
	AssertEqStr(g_foop, "blab");
	return 0;
}
SUITE_TEST(suite1,test2) {
	AssertEqStr(g_foop, "blab");
	return 0;
}
SUITE_DESTROY(suite1) {
	free(g_foop);
	return 0;
}
SUITE_SETUP(suite2) {
	return  strdup("blab");
}
SUITE_TEST_V(suite2,test2, local_ptr) {
	AssertEqStr((char *)local_ptr, "blab");
	return 0;
}
SUITE_DESTROYV(suite2, local_ptr) {
	free(local_ptr);
	return 0;
}
#ifdef __CYGWIN__
int main (int argc, char * argv[]){
	return windows_main(argc, argv);
}
#endif
