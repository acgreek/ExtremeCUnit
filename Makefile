ARCH := $(shell getconf LONG_BIT)

CFLAGS_DIST=
DIR_SUFFIX=
ifeq ($(ARCH), 32)
	LIBDIR=lib
else 
	LIBDIR=lib64
endif 
-include Makefile-common
