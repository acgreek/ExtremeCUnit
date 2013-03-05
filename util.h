#ifndef UTIL_H
#define UTIL_H
/** same as getenv, but returns the default if the env var is not defined or empty
 */
#include<stdlib.h>
const char * getenvd(const char *name, const char *default_str);
#endif
