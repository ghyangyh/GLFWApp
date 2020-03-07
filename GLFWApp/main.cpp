#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
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

// Global variables for mouse events
static float X_ROT_ANGLE(0.f), Y_ROT_ANGLE(0.f), Z_ROT_ANGLE(0.f);
static double LAST_MOUSE_POS_X(0.0), LAST_MOUSE_POS_Y(0.0);
static bool MOUSE_LEFT_BUTTON_DOWN(false);
static float MOUSE_MOVE_SPEED(0.1f);

static unsigned int CUBE_VAO(0);
static unsigned int CUBE_VBO(0), CUBE_EAO(0);
GLShaderProgram shader_program;
static bool RELOAD_PROGRAM_KEY_PRESSED(false);
static bool RELOAD_PROGRAM_KEY_1_PRESSED(false);
static bool RELOAD_PROGRAM_KEY_2_PRESSED(false);
// The model, view and projection matrix
static int MODEL_MAT_LOC(-1), VIEW_MAT_LOC(-1), PROJECTION_MAT_LOC(-1);
Eigen::Matrix4f MODEL_MAT, VIEW_MAT, PROJECTION_MAT;

// Non-shaded shader sources
//const string VERTEX_SHADER_FILE = "cube.vert";
//const string FRAGMENT_SHADER_FILE = "cube.frag";

// Phong lighting shader sources
//const string VERTEX_SHADER_FILE = "cube_phong.vert";
//const string FRAGMENT_SHADER_FILE = "cube_phong.frag";

// Blinn-Phong lighting shader sources
//const string VERTEX_SHADER_FILE = "cube_blinn_phong.vert"; // The vertex shader is the same
//const string FRAGMENT_SHADER_FILE = "cube_blinn_phong.frag";

vector<string> VERTEX_SHADER_FILES{"cube_phong.vert", "cube_blinn_phong.vert" };
vector<string> FRAGMENT_SHADER_FILES{ "cube_phong.frag", "cube_blinn_phong.frag" };
vector<string> SHADER_PROGRAM_TYPE{"Phong", "Blinn-Phong"};
int SHADER_PROGRAM_ID(0);

void setup_cube() {

	// a cube
	//float cube_vertices[] = {
	//	-5.f, 5.f, -5.f, 1.f, 0.f, 0.f,		// v0, color
	//	5.f, 5.f, -5.f,	0.f, 1.f, 0.f,		// v1, color
	//	5.f, 5.f, 5.f,	0.f, 0.f, 1.f,		// v2, color
	//	-5.f, 5.f, 5.f,	1.f, 0.f, 1.f,		// v3, color
	//	-5.f, -5.f, -5.f, 1.f, 1.f, 0.f,	// v4, color
	//	5.f, -5.f, -5.f, 0.f, 1.f, 1.f,		// v5, color
	//	5.f, -5.f, 5.f,	0.25f, 0.3f, 0.6f,	// v6, color
	//	-5.f, -5.f, 5.f, 1.f, 0.2f, 0.1f	// v7, color
	//};

	//float cube_vertices[] = {
	//	-5.f, 5.f, -5.f, 0.f, 1.f, 0.f,		// v0, normal
	//	5.f, 5.f, -5.f,	0.f, 1.f, 0.f,		// v1, normal
	//	5.f, 5.f, 5.f,	0.f, 1.f, 0.f,		// v2, normal
	//	-5.f, 5.f, 5.f,	0.f, 1.f, 0.f,		// v3, normal
	//	-5.f, -5.f, -5.f, 0.f, -1.f, 0.f,	// v4, normal
	//	5.f, -5.f, -5.f, 0.f, -1.f, 0.f,	// v5, normal
	//	5.f, -5.f, 5.f,	0.f, -1.f, 0.f,		// v6, normal
	//	-5.f, -5.f, 5.f, 0.f, -1.f, 0.f		// v7, normal
	//};

	float cube_vertices[] = {
		-5.f, 5.f, -5.f, 0.f, 1.f, 0.f,		// v0, normal(top face)
		5.f, 5.f, -5.f,	0.f, 1.f, 0.f,		// v1, normal(top face)
		-5.f, 5.f, 5.f,	0.f, 1.f, 0.f,		// v3, normal(top face)
		5.f, 5.f, -5.f,	0.f, 1.f, 0.f,		// v1, normal(top face)
		5.f, 5.f, 5.f,	0.f, 1.f, 0.f,		// v2, normal(top face)
		-5.f, 5.f, 5.f,	0.f, 1.f, 0.f,		// v3, normal(top face)

		5.f, -5.f, -5.f, 0.f, -1.f, 0.f,	// v5, normal(bottom face)
		-5.f, -5.f, -5.f, 0.f, -1.f, 0.f,	// v4, normal(bottom face)
		5.f, -5.f, 5.f,	0.f, -1.f, 0.f,		// v6, normal(bottom face)
		-5.f, -5.f, -5.f, 0.f, -1.f, 0.f,	// v4, normal(bottom face)
		-5.f, -5.f, 5.f, 0.f, -1.f, 0.f,	// v7, normal(bottom face)
		5.f, -5.f, 5.f,	0.f, -1.f, 0.f,		// v6, normal(bottom face)

		-5.f, 5.f, 5.f,	0.f, 0.f, 1.f,		// v3, normal(front face)
		5.f, 5.f, 5.f,	0.f, 0.f, 1.f,		// v2, normal(front face)
		-5.f, -5.f, 5.f, 0.f, 0.f, 1.f,		// v7, normal(front face)
		5.f, 5.f, 5.f,	0.f, 0.f, 1.f,		// v2, normal(front face)
		5.f, -5.f, 5.f,	0.f, 0.f, 1.f,		// v6, normal(front face)
		-5.f, -5.f, 5.f, 0.f, 0.f, 1.f,		// v7, normal(front face)

		5.f, 5.f, -5.f,	0.f, 0.f, -1.f,		// v1, normal(back face)
		-5.f, 5.f, -5.f, 0.f, 0.f, -1.f,	// v0, normal(back face)
		5.f, -5.f, -5.f, 0.f, 0.f, -1.f,	// v5, normal(back face)
		-5.f, 5.f, -5.f, 0.f, 0.f, -1.f,	// v0, normal(back face)
		-5.f, -5.f, -5.f, 0.f, 0.f, -1.f,	// v4, normal(back face)
		5.f, -5.f, -5.f, 0.f, 0.f, -1.f,	// v5, normal(back face)

		-5.f, 5.f, -5.f, -1.f, 0.f, 0.f,	// v0, normal(left face)
		-5.f, 5.f, 5.f,	-1.f, 0.f, 0.f,		// v3, normal(left face)
		-5.f, -5.f, -5.f, -1.f, 0.f, 0.f,	// v4, normal(left face)
		-5.f, 5.f, 5.f,	-1.f, 0.f, 0.f,		// v3, normal(left face)
		-5.f, -5.f, 5.f, -1.f, 0.f, 0.f,	// v7, normal(left face)
		-5.f, -5.f, -5.f, -1.f, 0.f, 0.f,	// v4, normal(left face)

		5.f, 5.f, 5.f,	1.f, 0.f, 0.f,		// v2, normal(right face)
		5.f, 5.f, -5.f,	1.f, 0.f, 0.f,		// v1, normal(right face)
		5.f, -5.f, 5.f,	1.f, 0.f, 0.f,		// v6, normal(right face)
		5.f, 5.f, -5.f,	1.f, 0.f, 0.f,		// v1, normal(right face)
		5.f, -5.f, -5.f, 1.f, 0.f, 0.f,	    // v5, normal(right face)
		5.f, -5.f, 5.f,	1.f, 0.f, 0.f		// v6, normal(right face)
	};

	int cube_face_indices[] = {
		0, 1, 2, 3, 4, 5,			// top face
		6, 7, 8, 9, 10, 11,			// bottom face
		12, 13, 14, 15, 16, 17,		// front face
		18, 19, 20, 21, 22, 23,		// back face
		24, 25, 26, 27, 28, 29,		// left face
		30, 31, 32, 33, 34, 35		// right face
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Fill in the cube face indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CUBE_EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_face_indices), cube_face_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void setup_mvp_matrices() {
	// The model matrix
	shader_program.get_uniform_location("aModelMat", MODEL_MAT_LOC);
	MODEL_MAT = Eigen::Matrix4f::Identity();//rotate_x(degree_to_radians(15.f));
	glUniformMatrix4fv(MODEL_MAT_LOC, 1, GL_FALSE, MODEL_MAT.data());

	// The viewing matrix
	shader_program.get_uniform_location("aViewMat", VIEW_MAT_LOC);
	Eigen::Vector3f CAM_POS(0.f, 0.f, 25.f);
	Eigen::Vector3f CAM_CENTER(0.f, 0.f, 0.f);
	Eigen::Vector3f CAM_UP(0.f, 1.f, 0.f);
	VIEW_MAT = view_transform(CAM_POS, CAM_CENTER, CAM_UP);
	glUniformMatrix4fv(VIEW_MAT_LOC, 1, GL_FALSE, VIEW_MAT.data());

	// The projection matrix
	float fovy_degree(60.f);
	float aspect(float(WINDOW_WIDTH) / float(WINDOW_HEIGHT));
	float z_near(0.01f), z_far(100.f);
	PROJECTION_MAT = perspective(fovy_degree, aspect, z_near, z_far);
	shader_program.get_uniform_location("aProjMat", PROJECTION_MAT_LOC);
	glUniformMatrix4fv(PROJECTION_MAT_LOC, 1, GL_FALSE, PROJECTION_MAT.data());
}

void update_fps_counter(GLFWwindow* pWindow) {
	double current_seconds(0.0), elapsed_seconds(0.0);
	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - PREVIOUS_SECONDS;
	if (elapsed_seconds > 0.25) { // 4 frames per second
		PREVIOUS_SECONDS = current_seconds;
		char tmp[128];
		double fps = double(FRAME_COUNT) / elapsed_seconds;
		sprintf(tmp, "opengl-%s@fps:%.2f", SHADER_PROGRAM_TYPE[SHADER_PROGRAM_ID].c_str(), fps);
		glfwSetWindowTitle(pWindow, tmp);
		FRAME_COUNT = 0;
	}
	++FRAME_COUNT;
}

void framebuffer_size_callback(GLFWwindow* pwindow, int width, int height) {
	glViewport(0, 0, width, height);
	
	// Update the projection matrix with new aspect ratio
	PROJECTION_MAT = perspective(60.f, float(width)/float(height), 0.01f, 100.f);
	shader_program.get_uniform_location("aProjMat", PROJECTION_MAT_LOC);
	glUniformMatrix4fv(PROJECTION_MAT_LOC, 1, GL_FALSE, PROJECTION_MAT.data());
}

void glfw_error_callback(int error, const char* description) {
	gl_log_err(GL_LOG_FILE, "GLFW ERROR: code %i msg: %s\n", error, description);
}

/* Callback for mouse buttons
*/
void glfw_mouse_button_callback(GLFWwindow* pwindow, int button, int action, int mods) {
	// If the left button is pressed, record the current mouse position
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(pwindow, &LAST_MOUSE_POS_X, &LAST_MOUSE_POS_Y);
		MOUSE_LEFT_BUTTON_DOWN = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		MOUSE_LEFT_BUTTON_DOWN = false;
	}
}

/* Callback for mouse movement
*/
void glfw_cursor_position_callback(GLFWwindow* pwindow, double xpos, double ypos) {
	if (MOUSE_LEFT_BUTTON_DOWN) {
		double dx = xpos - LAST_MOUSE_POS_X;
		double dy = ypos - LAST_MOUSE_POS_Y;
		
		LAST_MOUSE_POS_X = xpos;
		LAST_MOUSE_POS_Y = ypos;

		Y_ROT_ANGLE += MOUSE_MOVE_SPEED * dx;
		X_ROT_ANGLE += MOUSE_MOVE_SPEED * dy;
		
		normalize_angle_degrees(Y_ROT_ANGLE);
		normalize_angle_degrees(X_ROT_ANGLE);

	}
}

/* Callback for keyboard inputs.
*/
void processInput(GLFWwindow* pwindow) {
	if (glfwGetKey(pwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(pwindow, true);
	}
	// Reload the shader program if 'R' is pressed
	else if (glfwGetKey(pwindow, GLFW_KEY_R)==GLFW_PRESS  && !RELOAD_PROGRAM_KEY_PRESSED) {
		RELOAD_PROGRAM_KEY_PRESSED = true;
		cout << "Old shader program id: " << shader_program.get_shader_program_id() << endl;
		if (shader_program.reload()) {
			shader_program.use();
			// Reset these matrices
			setup_mvp_matrices();
		}
		cout << "New shader program id: " << shader_program.get_shader_program_id() << endl;
	}
	else if (glfwGetKey(pwindow, GLFW_KEY_R) == GLFW_RELEASE && RELOAD_PROGRAM_KEY_PRESSED) {
		RELOAD_PROGRAM_KEY_PRESSED = false;
	}
	else if (glfwGetKey(pwindow, GLFW_KEY_1) == GLFW_PRESS && !RELOAD_PROGRAM_KEY_1_PRESSED) {
		RELOAD_PROGRAM_KEY_1_PRESSED = true;
		SHADER_PROGRAM_ID = 0;
		cout << "Loading shade program: " << SHADER_PROGRAM_TYPE[0] << endl;
		if (shader_program.reload(VERTEX_SHADER_FILES[0], FRAGMENT_SHADER_FILES[0])) {
			shader_program.use();
			setup_mvp_matrices();
		}
	}
	else if (glfwGetKey(pwindow, GLFW_KEY_1) == GLFW_RELEASE && RELOAD_PROGRAM_KEY_1_PRESSED) {
		RELOAD_PROGRAM_KEY_1_PRESSED = false;
	}
	else if (glfwGetKey(pwindow, GLFW_KEY_2) == GLFW_PRESS && !RELOAD_PROGRAM_KEY_2_PRESSED) {
		RELOAD_PROGRAM_KEY_2_PRESSED = true;
		SHADER_PROGRAM_ID = 1;
		cout << "Loading shade program: " << SHADER_PROGRAM_TYPE[1] << endl;
		if (shader_program.reload(VERTEX_SHADER_FILES[1], FRAGMENT_SHADER_FILES[1])) {
			shader_program.use();
			setup_mvp_matrices();
		}
	}
	else if (glfwGetKey(pwindow, GLFW_KEY_2) == GLFW_RELEASE && RELOAD_PROGRAM_KEY_2_PRESSED) {
		RELOAD_PROGRAM_KEY_2_PRESSED = false;
	}
}


int main() {

	cout << "Keyboard interactions: \n";
	cout << "	number 1-3: load different shader programs at runtime\n ";
	cout << "	R/r: reload the current shader program at runtime\n";
	cout << "	esc: exit the program\n";

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
	glfwSetCursorPosCallback(p_window, glfw_cursor_position_callback);
	glfwSetMouseButtonCallback(p_window, glfw_mouse_button_callback);
	// glew initialize
	glewExperimental = GL_TRUE;
	glewInit();

	log_gl_params(GL_LOG_FILE);
	
	/* Setup the shader program 
	*/
	shader_program.attach_vertex_shader(VERTEX_SHADER_FILES[0]);
	shader_program.attach_fragment_shader(FRAGMENT_SHADER_FILES[0]);
	shader_program.set_gl_logger_file_name(string(GL_LOG_FILE));
	shader_program.link();
	shader_program.use();
	
	/* Setup the scene data
	*/
	setup_cube();

	/* Setup the model, view and projection matrices
	*/
	setup_mvp_matrices();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(p_window)) {

		update_fps_counter(p_window);
		shader_program.use();
		// update the model transformations according to mouse inputs
		MODEL_MAT = rotate_y(degree_to_radians(Y_ROT_ANGLE)) * rotate_x(degree_to_radians(X_ROT_ANGLE));
		glUniformMatrix4fv(MODEL_MAT_LOC, 1, GL_FALSE, MODEL_MAT.data());

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
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