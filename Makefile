CFLAGS=-Wall -ggdb -fPIC -fprofile-arcs -ftest-coverage 
DEPDIR = .dep
OBJDIR = obj
INSTALL_PREFIX = /usr
CC=gcc
SRC=main.c runner.c findtest_names.c assert_support.c
MAKEDEPEND = gcc -M $(CFLAGS) -o $*.d $<
OBJ_FILE_NAMES=$(SRC:.c=.o)
OBJS=$(patsubst %,$(OBJDIR)/%,$(OBJ_FILE_NAMES))

SHARE_LIBRARY_TARGET = libExtremeCUnit.so
HEADER_FILE = ExtremeCUnit.h
PKGCONFIG_FILE = ExtremeCUnit.pc

UNIT_TEST_SRC= unittest_tests.c unittest_cpp_tests.cc

INC_FILE_NAMES=$(SRC:.c=.P)
INCLUDES=$(patsubst %,$(DEPDIR)/%,$(INC_FILE_NAMES))

all: run
-include $(INCLUDES)

$(OBJDIR)/%.o : %.c
	mkdir -p $(OBJDIR); \
	mkdir -p $(DEPDIR); \
	$(MAKEDEPEND); \
	sed -e 's/\(^[^:\.o \t]\)/$(OBJDIR)\/\1/' < $*.d > $(DEPDIR)/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' -e  's/\(^[^:.o \t]*\.o: .*\)/$(OBJDIR)\/\1/' < $*.d >> $(DEPDIR)/$*.P; \
	$(COMPILE.c) -o $@ $< ;\
	rm -f $*.d;  

$(SHARE_LIBRARY_TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ -shared $(OBJS) 

install: $(SHARE_LIBRARY_TARGET)
	mkdir -p $(INSTALL_PREFIX)/lib
	mkdir -p $(INSTALL_PREFIX)/include	
	mkdir -p $(INSTALL_PREFIX)/lib/pkgconfig
	cp $(SHARE_LIBRARY_TARGET) $(INSTALL_PREFIX)/lib
	cp $(HEADER_FILE) $(INSTALL_PREFIX)/include
	cp $(PKGCONFIG_FILE) $(INSTALL_PREFIX)/lib/pkgconfig

test: $(UNIT_TEST_SRC) $(SHARE_LIBRARY_TARGET)
	$(CC) -ldl -pie -rdynamic $(CFLAGS) -Wl,--rpath,. -DUNIT_TEST -o $@ $< $(SHARE_LIBRARY_TARGET) -ldl  -lstdc++

.PHONY: clean clean_profiling run coverage install

clean_profiling:
	find . -name '*.gcda' | xargs -r rm 
#	find . -name '*.gcno' | xargs -r rm

run: test clean_profiling
	./test

runv: test clean_profiling
	./test -v

coverage:test clean_profiling
	lcov --directory obj --zerocounters
	./test
	lcov --directory obj --capture --output-file obj/app.info
	genhtml obj/app.info -o html

clean:  clean_profiling
	rm $(OBJS) test  $(SHARE_LIBRARY_TARGET) -rf
