#include "unittest.h"
#include <stdio.h>
TEST(test_1) {
	Assert(1==1);
	return 0;
}
TEST(test_2) {
	//Assert(1==0);
	return 0;
}
void chomp(char * str);
#include <string.h>
TEST(chomp_test_remove) {
	char foo[200] = "a\n";
	chomp(foo);
	Assert(0 == strcmp(foo,"a"));
	return 0;
}
TEST(int_test) {
	AssertEqInt(10 , 0xa);
	return 0;
}
#include <stdlib.h>
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
char *g_foop = 0;

SUITE_SETUP(suite1) {
	g_foop = strdup("blab");
	return 0;
}
SUITE_TEST(suite1,test1) {
	AssertEqStr(g_foop, "blab");
	return 0;
}

SUITE_DESTROY(suite1) {
	free(g_foop); 
	return 0;
}

SUITE_SETUP(suite2) {
	g_foop = strdup("blab");
	return 0;
}
SUITE_TEST(suite2,test2) {
	AssertEqStr(g_foop, "blab");
	return 0;
}

SUITE_DESTROY(suite2) {
	free(g_foop); 
	return 0;
}
