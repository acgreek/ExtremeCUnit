#include "ExtremeCUnit.h"
#include <string>
TEST(chomp_test_remove_cpp) {
	std::string str= "a\n";
	Assert(0 == str.compare(str));
	return 0;
}
TEST(throw_test) {
	throw std::string("blue bear");
	return 0;
}
SUITE_SETUP(CPP_suite) {
	return NULL;
}
