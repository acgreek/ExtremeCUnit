CFLAGS=-ggdb -fPIC
CC=gcc
SRC=main.c runner.c
OBJS=$(SRC:.c=.o)


libunittest.so:	$(OBJS)
	$(CC) $(CFLGAS) -o libunittest.so -shared  $(OBJS)

test: unittest_tests.c libunittest.so
	$(CC) $(CFLAGS) -DUNIT_TEST -o $@ unittest_tests.c libunittest.so
	./$@
	
clean:  
	rm $(OBJS) test libunitest.so 
