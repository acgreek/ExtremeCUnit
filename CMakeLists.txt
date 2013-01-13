cmake_minimum_required (VERSION 0.9.1)
project (ExtremeCUnit)

find_package(PkgConfig)
set(CMAKE_C_FLAGS "-Wall -ggdb3 -fPIC " )
set(CMAKE_CXX_FLAGS "-std=c++0x " )

add_library(ExtremeCUnit SHARED main.cc runner.c findtest_names.c assert_support.c)
set(CMAKE_EXE_LINKER_FLAGS "-pie -rdynamic  -shared -ldl -Wl,--rpath,.,--rpath,dist")

add_library(CPP_tests SHARED unittest_cpp_tests.cc)

add_executable(test unittest_tests.c unittest_cpp_tests.cc ) 

target_link_libraries(test CPP_tests ExtremeCUnit dl stdc++)
set_target_properties(test PROPERTIES CMAKE_C_LINK_FLAGS "${CMAKE_C_FLAGS} -DUNIT_TEST")
set_target_properties(test PROPERTIES CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_FLAGS}  -DUNIT_TEST")

add_custom_target(run ./test)
add_custom_target(runv ./test -v)
ADD_DEPENDENCIES(run test)
ADD_DEPENDENCIES(runv test)
