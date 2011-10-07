#include "unittest.h"
#include <stdio.h>
TEST(test_1) {
	printf("asdsd");
	Assert(1==1);
	return 0;
}
TEST(test_2) {
	Assert(0==1);
	return 1;
}
