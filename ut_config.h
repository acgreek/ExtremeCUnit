#ifndef UT_CONFIG_H
#define UT_CONFIG_H
typedef struct _ut_configuration_t {
	char * program_name;
	int output_fd;
	void  *dynlibraryp;
} ut_configuration_t;
#define UT_CONFIGURATION_DEFAULT {argv[0], 2}
#endif
