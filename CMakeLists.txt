cmake_minimum_required (VERSION 0.9.1)
project (ExtremeCUnit)

cmake_policy(SET CMP0003 NEW)


find_package(PkgConfig)
set(CMAKE_C_FLAGS "-Wall -ggdb3 -fPIC " )
set(CMAKE_CXX_FLAGS "-std=c++0x" )

add_library(ExtremeCUnit SHARED main.c runner.c findtest_names.c assert_support.c)
set(CMAKE_EXE_LINKER_FLAGS "-pie -rdynamic -ldl -lstdc++")

IF(CYGWIN)
	set(CMAKE_C_FLAGS "-Wl,--export-all-symbols ${CMAKE_C_FLAGS}" )
	set(CMAKE_CXX_FLAGS "-Wl,--export-all-symbols ${CMAKE_CXX_FLAGS}" )
	set(CMAKE_CXX_LINK_FLAGS "-Wl,--export-all-symbols ${CMAKE_CXX_LINK_FLAG}" )
ENDIF(${CYGWIN} MATCHES "TRUE")

set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_C_COMPILER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

add_executable(test unittest_tests.c unittest_cpp_tests.cc ) 

target_link_libraries(test ExtremeCUnit dl stdc++)
set_target_properties(test PROPERTIES CMAKE_C_LINK_FLAGS "${CMAKE_C_FLAGS} -DUNIT_TEST")
set_target_properties(test PROPERTIES CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_FLAGS}  -DUNIT_TEST")

add_custom_target(run ./test)
add_custom_target(runv ./test -v)
ADD_DEPENDENCIES(run test)
ADD_DEPENDENCIES(runv test)

install(TARGETS ExtremeCUnit DESTINATION "lib")
install(FILES ExtremeCUnit.h DESTINATION "include")

