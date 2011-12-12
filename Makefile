CFLAGS=-Wall -ggdb -fPIC -fprofile-arcs -ftest-coverage 
DEPDIR = .dep
OBJDIR = obj
CC=gcc
SRC=main.c runner.c findtest_names.c assert_support.c
MAKEDEPEND = gcc -M $(CFLAGS) -o $*.d $<
OBJ_FILE_NAMES=$(SRC:.c=.o)
OBJS=$(patsubst %,$(OBJDIR)/%,$(OBJ_FILE_NAMES))

INC_FILE_NAMES=$(SRC:.c=.P)
INCLUDES=$(patsubst %,$(DEPDIR)/%,$(INC_FILE_NAMES))

-include $(INCLUDES)
all: run

$(OBJDIR)/%.o : %.c
	mkdir -p $(OBJDIR); \
	mkdir -p $(DEPDIR); \
	$(MAKEDEPEND); \
	sed -e 's/\(^[^:\.o \t]\)/$(OBJDIR)\/\1/' < $*.d > $(DEPDIR)/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' -e  's/\(^[^:.o \t]*\.o: .*\)/$(OBJDIR)\/\1/' < $*.d >> $(DEPDIR)/$*.P; \
	$(COMPILE.c) -o $@ $< ;\
	rm -f $*.d;  

libunittest.so:	$(OBJS)
	$(CC) $(CFLAGS) -o libunittest.so -shared  $(OBJS) 

test: unittest_tests.c libunittest.so
	$(CC) -ldl -pie -rdynamic $(CFLAGS) -Wl,--rpath,. -DUNIT_TEST -o $@ unittest_tests.c libunittest.so  -ldl 

.PHONY: clean clean_profiling run coverage

clean_profiling:
	find . -name '*.gcda' | xargs -r rm 
#	find . -name '*.gcno' | xargs -r rm

run: test clean_profiling
	./test


coverage:test clean_profiling
	lcov --directory obj --zerocounters
	./test
	lcov --directory obj --capture --output-file obj/app.info
	genhtml obj/app.info -o html

clean:  clean_profiling
	rm $(OBJS) test libunittest.so  -rf
