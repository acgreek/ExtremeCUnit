// vim: set sw=4 ts=4 sts=4 noet tw=78  foldlevel=0 foldmethod=syntax spell:
#define UNIT_TEST
#include "ExtremeCUnit.h"
#include <string>

TEST(chomp_test_remove_cpp) {
	std::string str= "a\n";
	Assert(0 == str.compare("a\n"));
	return 0;
}
static void foo () {
	throw "bla";
}
TEST(throw_test) {
	bool exception_thrown= false;
	try {
		foo();
	}
	catch (...) {
		exception_thrown=true;
	}
	Assert(true == exception_thrown );
	return 0;
}
SUITE_SETUP(CPP_suite) {
	return NULL;
}
