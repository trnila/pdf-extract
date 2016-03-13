#include <stdarg.h>
#include <stdio.h>
#include "log.h"

int verbosity = 0;

#define REGISTER_LOG(name, level) void log_##name(const char *message, ...) { \
	if(verbosity >= level) { \
		va_list args; \
		va_start(args, message); \
		vfprintf(stderr, message, args); \
		va_end(args); \
	} \
}

REGISTER_LOG(error, 0)
REGISTER_LOG(info, 1)
REGISTER_LOG(debug, 2)