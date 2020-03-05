#ifndef GL_SHADER_PROGRAM_H
#define GL_SHADER_PROGRAM_H

#include <string>

#include <GL/glew.h>

class GLShaderProgram {
	enum ShaderType {
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};
public:
	GLShaderProgram();
	GLShaderProgram(const std::string& aVertexShaderFileName, const std::string& aFragmentShaderFileName, const std::string& aLoggerFileName);
	~GLShaderProgram();
	GLShaderProgram(const GLShaderProgram&) = delete;
	GLShaderProgram& operator=(const GLShaderProgram&) = delete;
public:
	bool attach_vertex_shader(const std::string& aVertexShaderFileName);
	bool attach_fragment_shader(const std::string& aFragmentShaderFileName);
	bool link();
	inline void use() const {
		glUseProgram(m_program_id);
	}
	inline void set_gl_logger_file_name(const std::string& aFileName) {
		m_gl_logger_file_name = aFileName;
	}
	inline unsigned int get_shader_program_id() const {
		return m_program_id;
	}
	inline bool is_success() const {
		return m_b_success;
	}
	bool load_shader_file(const std::string& aFileName, std::string& aShaderSourceStr);
	int get_uniform_location(const std::string& aUniformName, int& aUniformLocation);

protected:
	int compile_shader(ShaderType aShaderType, const std::string& aShaderSourceStr);
	int link_program();

private:
	unsigned int m_vertex_shader_id;
	unsigned int m_fragment_shader_id;
	unsigned int m_program_id;
	bool m_b_success;
	std::string m_gl_logger_file_name;

};

#endif
