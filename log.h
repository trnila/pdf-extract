#pragma once
extern int verbosity;

void log_error(const char *message, ...);
void log_notice(const char *message, ...);
void log_debug(const char *message, ...);