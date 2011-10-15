CFLAGS=-Wall -ggdb -fPIC -fprofile-arcs -ftest-coverage -lgcov

DEPDIR = .dep
OBJDIR = obj
CC=gcc
SRC=main.c runner.c findtest_names.c assert_support.c
MAKEDEPEND = gcc -M $(CFLAGS) -o $*.d $<
OBJS=$(SRC:.c=.o)

-include $(patsubstr %, $(DEPDIR)/%, $(SRC:.c=.P))

%.o : %.c
	@$(MAKEDEPEND); \
	mkdir -p $(DEPDIR)
	cp $*.d $(DEPDIR)/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(DEPDIR)/$*.P; \
	rm -f $*.d
	$(COMPILE.c) -o $@ $<


libunittest.so:	$(OBJS)
	$(CC) $(CFLAGS) -o libunittest.so -shared  $(OBJS) 

test: unittest_tests.c libunittest.so
	$(CC) -ldl -pie -rdynamic $(CFLAGS) -Wl,--rpath,. -DUNIT_TEST -o $@ unittest_tests.c libunittest.so  -ldl


clean_profiling:
	find . -name '*.gcda' | xargs -r rm 
	find . -name '*.gcno' | xargs -r rm

run: test clean_profiling
	./test
clean:  clean_profiling
	rm $(OBJS) test libunittest.so  -rf
