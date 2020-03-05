#ifndef GL_SHADER_PROGRAM_H
#define GL_SHADER_PROGRAM_H

#include <string>

#include <GL/glew.h>

class GLShaderProgram {
	/* Shader types can be expanded to include other shaders in demand.
	*/
	enum ShaderType {
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};
public:
	GLShaderProgram();
	/* This constructor will compile and link the vertex & fragment shaders into a shader program.
	*/
	GLShaderProgram(const std::string& aVertexShaderFileName, const std::string& aFragmentShaderFileName, const std::string& aLoggerFileName);
	~GLShaderProgram();
	/* The copy and assignment is disabled
	*/
	GLShaderProgram(const GLShaderProgram&) = delete;
	GLShaderProgram& operator=(const GLShaderProgram&) = delete;
public:
	/* Load a vertex shader source from file, then compile it. 
	* Errors will be recorded if a logger file name is provided.
	* aVertexShaderFileName: the vertex shader source file name.
	*/
	bool attach_vertex_shader(const std::string& aVertexShaderFileName);

	/* Load a fragment shader source from file, then compile it.
	* Errors will be recorded if a logger file name is provided.
	* aFragmentShaderFileName: the fragment shader source file name.
	*/
	bool attach_fragment_shader(const std::string& aFragmentShaderFileName);

	/* Link the compiled shader objects into a final shader program.
	*/
	bool link();

	/* Set the shader program as the current one to render a scene.
	*/
	inline void use() const {
		glUseProgram(m_program_id);
	}

	/* Set the logger file's name.
	*/
	inline void set_gl_logger_file_name(const std::string& aFileName) {
		m_gl_logger_file_name = aFileName;
	}

	
	inline unsigned int get_shader_program_id() const {
		return m_program_id;
	}

	inline bool is_success() const {
		return m_b_success;
	}

	/* Utility function to load a shader source string from a file.
	* aFileName: the shader source file name.
	* aShaderSourceStr: the shader source is written into this string.
	*/
	bool load_shader_file(const std::string& aFileName, std::string& aShaderSourceStr);

	/* Utility function to get a uniform variable's location in current shader program.
	* aUniformName: the uniform variable's name in the shader source.
	* aUniformLocation: the uniform variable's location is stored in this variable.
	* return -1 for an error, otherwise 1.
	*/
	int get_uniform_location(const std::string& aUniformName, int& aUniformLocation);

protected:

	/* Utility function to compile a shader from a source string.
	* Errors will be recorded to a logger file, if such a file is provided.
	* aShaderType: a ShaderType's variable to indicate the shader type.
	* aShaderSourceStr: the shader source string to be compiled.
	* returns 0 if failed.
	*/
	int compile_shader(ShaderType aShaderType, const std::string& aShaderSourceStr);

	/* Utility function to link all the compiled shaders into one program.
	* returns 0 if failed.
	*/
	int link_program();

private:
	unsigned int m_vertex_shader_id; 
	unsigned int m_fragment_shader_id;
	unsigned int m_program_id;
	bool m_b_success;
	std::string m_gl_logger_file_name;

};

#endif
