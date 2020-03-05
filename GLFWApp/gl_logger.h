#ifndef GL_LOGGER_H
#define GL_LOGGER_H

#include <cstdarg>

bool restart_gl_log(const char* aLogFileName);
bool gl_log(const char* aLogFileName, const char* aMessage, ...);
bool gl_log_err(const char* aLogFileName, const char* aMessage, ...);
void log_gl_params(const char* aLogFileName);


#endif