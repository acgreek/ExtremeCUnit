#include "unittest.h"
#include <stdio.h>
TEST(test_1) {
	Assert(1==1);
	return 0;
}
TEST(test_2) {
	Assert(1==0);
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
//	char foo[200] = "a";
	//AssertEqInt(strcmp(foo,"a"), 0);
	return 0;
}
