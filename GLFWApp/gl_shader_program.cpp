#include "gl_shader_program.h"


#include "gl_logger.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

GLShaderProgram::GLShaderProgram():m_vertex_shader_id(0), 
m_fragment_shader_id(0), 
m_program_id(0),
m_b_success(false),
m_gl_logger_file_name(""),
m_vertex_shader_file_name(""),
m_fragment_shader_file_name("")
{

}

GLShaderProgram::GLShaderProgram(const string& aVertexShaderFileName,
	const string& aFragmentShaderFileName,
	const string& aLoggerFileName) :
	m_vertex_shader_id(0), 
	m_fragment_shader_id(0), 
	m_program_id(0), 
	m_b_success(false),
	m_gl_logger_file_name(aLoggerFileName), 
	m_vertex_shader_file_name(aVertexShaderFileName), 
	m_fragment_shader_file_name(aFragmentShaderFileName) {
	
	// Attach and compile a vertex shader code
	m_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	m_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	string a_vertex_shader_source_str, a_fragment_shader_source_str;
	// Load the shader souce files
	if (load_shader_file(aVertexShaderFileName, a_vertex_shader_source_str) &&
		load_shader_file(aFragmentShaderFileName, a_fragment_shader_source_str)) {
		
		// Compile the shaders
		if(compile_shader(VERTEX_SHADER, a_vertex_shader_source_str) &&
			compile_shader(FRAGMENT_SHADER, a_fragment_shader_source_str)) {

			// Link a shader program
			if ((m_program_id = link_program()) != 0) {
				m_b_success = true;
				glDeleteShader(m_vertex_shader_id);
				glDeleteShader(m_fragment_shader_id);
			}
			else
				m_b_success = false;
						}
		else
			m_b_success = false;
	}
	else {
		m_b_success = false;
	}
}

GLShaderProgram::~GLShaderProgram() {
	if (m_b_success)
		glDeleteProgram(m_program_id);
}

bool GLShaderProgram::attach_vertex_shader(const string& aVertexShaderFileName) {
	// Store the vertex shader file name
	m_vertex_shader_file_name = aVertexShaderFileName;

	// Create a vertex shader id
	m_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	string a_vertex_shader_source_str;
	// Read and compile the shader source from file
	if (load_shader_file(aVertexShaderFileName, a_vertex_shader_source_str) &&
		compile_shader(VERTEX_SHADER, a_vertex_shader_source_str) ) 
			return true;

	return false;
}

bool GLShaderProgram::attach_fragment_shader(const string& aFragmentShaderFileName) {
	m_fragment_shader_file_name = aFragmentShaderFileName;
	m_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	string a_fragment_shader_source_str;
	if (load_shader_file(aFragmentShaderFileName, a_fragment_shader_source_str) &&
		compile_shader(FRAGMENT_SHADER, a_fragment_shader_source_str))
		return true;
	return false;
}

bool GLShaderProgram::link() {
	if ((m_program_id = link_program()) != 0)
		m_b_success = true;
	else
		m_b_success = false;
	glDeleteShader(m_vertex_shader_id);
	glDeleteShader(m_fragment_shader_id);
	return m_b_success;
}

bool GLShaderProgram::reload() {
	if (attach_vertex_shader(m_vertex_shader_file_name)) {
		if (attach_fragment_shader(m_fragment_shader_file_name)) {
			unsigned int new_program_id = link_program();
			if (new_program_id != 0) {
				glDeleteProgram(m_program_id);
				glDeleteShader(m_vertex_shader_id);
				glDeleteShader(m_fragment_shader_id);

				m_program_id = new_program_id;
				return true;
			}
			else {
				gl_log_err("ERROR: Linking the shader program failed after reloading shaders: %s, %s\n",
					m_vertex_shader_file_name.c_str(), m_fragment_shader_file_name.c_str());
				return false;
			}
		}
		else {
			gl_log_err("ERROR: Compiling the fragment shader failed when reloading %s\n",
				m_vertex_shader_file_name.c_str());
			return false;
		}
	}
	else {
		gl_log_err("ERROR: Compiling the vertex shader failed when reloading %s\n",
			m_vertex_shader_file_name.c_str());
		return false;
	}
}

bool GLShaderProgram::load_shader_file(const string& aFileName, string& aShaderSourceStr) {
	ifstream inputs(aFileName);
	if (inputs) {
		stringstream ss;
		ss << inputs.rdbuf();
		aShaderSourceStr = ss.str();
		return true;
	}
	if (!m_gl_logger_file_name.empty()) {
		gl_log(m_gl_logger_file_name.c_str(), 
			"ERROR: Failed open the shader file \" %s! \" \n", 
			aFileName.c_str());
	}
	return false;
}

int GLShaderProgram::compile_shader(ShaderType aShaderType,
	const string& aShaderSourceStr) {
	int b_success(0);
	char info_buf[512];
	const char* a_shader_source_str = aShaderSourceStr.c_str();
	switch (aShaderType) {
	case VERTEX_SHADER:
		glShaderSource(m_vertex_shader_id, 1, &(a_shader_source_str), nullptr);
		glCompileShader(m_vertex_shader_id);
		glGetShaderiv(m_vertex_shader_id, GL_COMPILE_STATUS, &b_success);
		if (!b_success && !m_gl_logger_file_name.empty()) {
			glGetShaderInfoLog(m_vertex_shader_id, 512, nullptr, info_buf);
			gl_log_err(m_gl_logger_file_name.c_str(),
				"VERTEX_SHADER_COMPILATIONA ERROR: %s\n", info_buf);
		}
		break;
	case FRAGMENT_SHADER:
		glShaderSource(m_fragment_shader_id, 1, &(a_shader_source_str), nullptr);
		glCompileShader(m_fragment_shader_id);
		glGetShaderiv(m_fragment_shader_id, GL_COMPILE_STATUS, &b_success);
		if (!b_success && !m_gl_logger_file_name.empty()) {
			glGetShaderInfoLog(m_fragment_shader_id, 512, nullptr, info_buf);
			gl_log_err(m_gl_logger_file_name.c_str(),
				"FRAGMENT_SHADER_COMPILATIONA ERROR: %s\n", info_buf);
		}
		break;
	default:
		if (!m_gl_logger_file_name.empty()) {
			gl_log_err(m_gl_logger_file_name.c_str(), "ERROR: SHADER TYPE NOT SUPPORTED!\n");
			b_success = 0;
		}
		break;
	}
	return b_success;
}

unsigned int GLShaderProgram::link_program() {
	unsigned int program_id = glCreateProgram();
	glAttachShader(program_id, m_vertex_shader_id);
	glAttachShader(program_id, m_fragment_shader_id);
	glLinkProgram(program_id);
	int b_success(0);
	char info_buf[512];
	glGetProgramiv(program_id, GL_LINK_STATUS, &b_success);
	if (!b_success && !m_gl_logger_file_name.empty()) {
		glGetProgramInfoLog(program_id, 512, nullptr, info_buf);
		gl_log_err(m_gl_logger_file_name.c_str(),
			"SHADER_PROGRAM_LINK_ERROR: %s\n",
			info_buf);
		return 0;
	}
	return program_id;
}

int GLShaderProgram::get_uniform_location(const string& aUniformName, int& aUniformLocation) {
	aUniformLocation = glGetUniformLocation(m_program_id, aUniformName.c_str());
	if (aUniformLocation == -1) {
		gl_log_err(m_gl_logger_file_name.c_str(), "ERROR: failed get the uniform variable %s!\n", aUniformName.c_str());
		return -1;
	}
	return 1;
}