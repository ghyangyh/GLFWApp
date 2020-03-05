#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

#include "GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"


#include "gl_logger.h"
#include "transformation_3d.h"
#include "gl_shader_program.h"

static const char* GL_LOG_FILE = "gl_log.txt";
static double PREVIOUS_SECONDS(0.0);
static int FRAME_COUNT(0);
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

static float X_ROT_ANGLE(0.f), Y_ROT_ANGLE(0.f), Z_ROT_ANGLE(0.f);

static unsigned int CUBE_VAO(0);
static unsigned int CUBE_VBO(0), CUBE_EAO(0);
GLShaderProgram shader_program;



void update_fps_counter(GLFWwindow* pWindow) {
	double current_seconds(0.0), elapsed_seconds(0.0);
	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - PREVIOUS_SECONDS;
	if (elapsed_seconds > 0.25) { // 4 frames per second
		PREVIOUS_SECONDS = current_seconds;
		char tmp[128];
		double fps = double(FRAME_COUNT) / elapsed_seconds;
		sprintf(tmp, "opengl@fps:%.2f", fps);
		glfwSetWindowTitle(pWindow, tmp);
		FRAME_COUNT = 0;
	}
	++FRAME_COUNT;
}

void framebuffer_size_callback(GLFWwindow* pwindow, int width, int height) {
	glViewport(0, 0, width, height);
	Eigen::Matrix4f perspective_mat = perspective(60.f, float(width)/float(height), 0.01f, 100.f);
	int projection_mat_loc;
	shader_program.get_uniform_location("aProjMat", projection_mat_loc);
	glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, perspective_mat.data());
}

void glfw_error_callback(int error, const char* description) {
	gl_log_err(GL_LOG_FILE, "GLFW ERROR: code %i msg: %s\n", error, description);
}

void processInput(GLFWwindow* pwindow) {
	if (glfwGetKey(pwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(pwindow, true);
	}
}

const string VERTEX_SHADER_FILE = "cube.vert";
const string FRAGMENT_SHADER_FILE = "cube.frag";

int main() {
	// start the gl logger
	if (!restart_gl_log(GL_LOG_FILE))
		return 1;
	gl_log(GL_LOG_FILE, "starting GLFW\n%s\n", glfwGetVersionString());

	// register the error callback function
	glfwSetErrorCallback(glfw_error_callback);

	// init the glfw lib
	if (!glfwInit()) {
		cerr << "ERROR: could not start GLFW3\n";
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* p_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Triangle", nullptr, nullptr);
	if (p_window == nullptr) {
		cerr << "ERROR: failed create a glfw window!\n";
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(p_window);
	glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);

	// glew initialize
	glewExperimental = GL_TRUE;
	glewInit();

	log_gl_params(GL_LOG_FILE);
	
	/* Setup the shader program 
	*/
	shader_program.attach_vertex_shader(VERTEX_SHADER_FILE);
	shader_program.attach_fragment_shader(FRAGMENT_SHADER_FILE);
	shader_program.set_gl_logger_file_name(string(GL_LOG_FILE));
	shader_program.link();
	shader_program.use();
	
	/* Setup the scene data
	*/
	// a cube
	float cube_vertices[] = {
		-5.f, 5.f, -5.f, 1.f, 0.f, 0.f,		// v0, color
		5.f, 5.f, -5.f,	0.f, 1.f, 0.f,		// v1, color
		5.f, 5.f, 5.f,	0.f, 0.f, 1.f,		// v2, color
		-5.f, 5.f, 5.f,	1.f, 0.f, 1.f,		// v3, color
		-5.f, -5.f, -5.f, 1.f, 1.f, 0.f,	// v4, color
		5.f, -5.f, -5.f, 0.f, 1.f, 1.f,		// v5, color
		5.f, -5.f, 5.f,	0.25f, 0.3f, 0.6f,	// v6, color
		-5.f, -5.f, 5.f, 1.f, 0.2f, 0.1f	// v7, color
	};

	int cube_face_indices[] = {
		0, 1, 3, 1, 2, 3, // top face
		5, 4, 6, 4, 7, 6, // bottom face
		3, 2, 7, 2, 6, 7, // front face
		1, 0, 5, 0, 4, 5, // back face
		0, 3, 4, 3, 7, 4, // left face
		2, 1, 6, 1, 5, 6 // right face
	};

	// Send the cube data to GPU
	glGenVertexArrays(1, &CUBE_VAO);
	glGenBuffers(1, &CUBE_VBO);
	glGenBuffers(1, &CUBE_EAO);

	glBindVertexArray(CUBE_VAO);

	// Fill in the vertex attributes buffer
	glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// Specify the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Specify the color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// Fill in the cube face indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CUBE_EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_face_indices), cube_face_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	/* Setup the model, view and projection matrices
	*/
	int model_mat_loc;
	shader_program.get_uniform_location("aModelMat", model_mat_loc);
	Eigen::Matrix4f model_mat = rotate_x(degree_to_radians(15.f));
	glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_mat.data());

	// The viewing matrix
	int view_mat_loc;
	shader_program.get_uniform_location("aViewMat", view_mat_loc);
	Eigen::Vector3f CAM_POS(0.f, 0.f, 25.f);
	Eigen::Vector3f CAM_CENTER(0.f, 0.f, 0.f);
	Eigen::Vector3f CAM_UP(0.f, 1.f, 0.f);
	Eigen::Matrix4f view_mat = view_transform(CAM_POS, CAM_CENTER, CAM_UP);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view_mat.data());

	// The projection matrix
	float fovy_degree(60.f);
	float aspect(float(WINDOW_WIDTH) / float(WINDOW_HEIGHT));
	float z_near(0.01f), z_far(100.f);
	Eigen::Matrix4f perspective_mat = perspective(fovy_degree, aspect, z_near, z_far);
	int projection_mat_loc;
	shader_program.get_uniform_location("aProjMat", projection_mat_loc);
	glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, perspective_mat.data());

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(p_window)) {

		update_fps_counter(p_window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_program.use();
		glBindVertexArray(CUBE_VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// process input
		processInput(p_window);

		// glfw swapbuffers and poll IO events
		glfwSwapBuffers(p_window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}