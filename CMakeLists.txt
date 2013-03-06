cmake_minimum_required (VERSION 2.8.4)
project (ExtremeCUnit)

cmake_policy(SET CMP0003 NEW)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 9.1)

find_package(PkgConfig)
set(CMAKE_C_FLAGS "-Wall -Wextra -ggdb3" )
set(CMAKE_CXX_FLAGS "-std=c++0x" )

add_library(ExtremeCUnit SHARED main.c runner.c util.c findtest_names.c assert_support.c)

IF(CYGWIN)
	set(CMAKE_C_FLAGS "-Wl,--export-all-symbols ${CMAKE_C_FLAGS}" )
	set(CMAKE_CXX_FLAGS "-Wl,--export-all-symbols ${CMAKE_CXX_FLAGS}" )
	set(CMAKE_CXX_LINK_FLAGS "-Wl,--export-all-symbols ${CMAKE_CXX_LINK_FLAG}" )

	set (PC_LIBRARY_FLAGS "-Wl,--export-all-symbols -Wl,--rpath,.,--rpath,dist,--rpath,${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib/libExtremeCUnit.dll.a -ldl ")
	set (PC_C_FLAGS "-DUNIT_TEST -pie ")
	set(CMAKE_EXE_LINKER_FLAGS "-pie -ldl -lstdc++")
ELSE()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC " )
	set(CMAKE_EXE_LINKER_FLAGS "-pie -rdynamic -ldl -lstdc++")
	set (PC_LIBRARY_FLAGS "-pie -Wl,--rpath,.,--rpath,dist -lExtremeCUnit -ldl" )
	set (PC_C_FLAGS "-DUNIT_TEST -fPIC -pie -rdynamic" )
ENDIF(CYGWIN)


configure_file (
	"${PROJECT_SOURCE_DIR}/ExtremeCUnit.pc.in"
   "${PROJECT_BINARY_DIR}/ExtremeCUnit.pc"
)

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
install(FILES ExtremeCUnit.pc DESTINATION "lib/pkgconfig")

