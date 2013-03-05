// vim: ts=2 noet spell foldmethod=syntax :
#include "util.h"
const char * getenvd(const char *name, const char *default_str) {
		char * result = getenv(name);
		if (NULL == result || '\0'== result)
				return default_str;
		return result;
}
