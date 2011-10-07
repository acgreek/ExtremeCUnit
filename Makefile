CFLAGS=-Wall -ggdb -fPIC
CC=gcc
SRC=main.c runner.c findtest_names.c
OBJS=$(SRC:.c=.o)


libunittest.so:	$(OBJS)
	$(CC) $(CFLGAS) -o libunittest.so -shared  $(OBJS)

test: unittest_tests.c libunittest.so
	$(CC) $(CFLAGS) -Wl,--rpath,. -DUNIT_TEST -o $@ unittest_tests.c libunittest.so
	./test
	
clean:  
	rm $(OBJS) test libunittest.so  -rf
