#include "gl_logger.h"

#include <ctime>
#include <cstdio>
using namespace std;

#include <GL/glew.h>

bool restart_gl_log(const char* aLogFileName) {
	FILE* p_file = fopen(aLogFileName, "w");
	if (!p_file) {
		fprintf(stderr,
			"ERROR: could not open gl log file %s for writting\n",
			aLogFileName);
		return false;
	}
	time_t now = time(NULL);
	char* date = ctime(&now);
	fprintf(p_file, "gl log.local time %s\n", date);
	fclose(p_file);
	return true;
}



bool gl_log(const char* aLogFileName, const char* aMessage, ...) {
	va_list arg_list;
	FILE* p_file = fopen(aLogFileName, "a");
	if (!p_file) {
		fprintf(stderr,
			"ERROR: could not open gl log file %s for appending\n",
			aLogFileName);
		return false;
	}
	va_start(arg_list, aMessage);
	vfprintf(p_file, aMessage, arg_list);
	va_end(arg_list);
	fclose(p_file);
	return true;
}


bool gl_log_err(const char* aLogFileName, const char* aMessage, ...) {
	FILE* p_file = fopen(aLogFileName, "a");
	if (!p_file) {
		fprintf(stderr, 
			"ERROR: could not open gl log file %s for appending\n",
			aLogFileName);
		return false;
	}
	va_list arg_list;
	va_start(arg_list, aMessage);
	vfprintf(p_file, aMessage, arg_list);
	va_end(arg_list);
	va_start(arg_list, aMessage);
	fprintf(stderr, aMessage, arg_list);
	va_end(arg_list);
	fclose(p_file);
	return true;
}

void log_gl_params(const char* aLogFileName) {
	GLenum gl_params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO
	};
	const char* gl_param_names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO"
	};

	// Vendor information
	const GLubyte* gl_vendor_info = glGetString(GL_RENDERER);
	const GLubyte* gl_version_info = glGetString(GL_VERSION);

	gl_log(aLogFileName, "Renderer: %s\n", gl_vendor_info);
	gl_log(aLogFileName, "OpenGL version: %s\n", gl_version_info);

	gl_log(aLogFileName, "GL Context Params:\n");
	for (int i = 0; i < 10; ++i) {
		int value(0);
		glGetIntegerv(gl_params[i], &value);
		gl_log(aLogFileName, "%s:%i\n", gl_param_names[i], value);
	}
	int value_array[2] = { 0 };
	glGetIntegerv(gl_params[10], value_array);
	gl_log(aLogFileName,
		"%s:%i,%i\n",
		gl_param_names[10],
		value_array[0],
		value_array[1]
	);
	unsigned char s(0);
	glGetBooleanv(gl_params[11], &s);
	gl_log(aLogFileName, "%s:%u\n", gl_param_names[11], (unsigned int)s);
	gl_log(aLogFileName, "-------------------------------------\n");
}