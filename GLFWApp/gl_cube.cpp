#include "gl_cube.h"

#include <GL/glew.h>

#include <iostream>
using namespace std;

GLCube::GLCube(float aCubeWidth):m_width(aCubeWidth),
								m_cube_face_indices{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35},
								m_vao(0),
								m_vbo(0),
								m_eao(0) {
	m_vertex_attribute_array.reserve(m_array_size);
	fill_vertex_attribute_array();
	cout << "Cube vertices: " << m_cube_face_indices.size() << endl;
	cout << "Cube triangles: " << m_cube_face_indices.size() / 3 << endl;
	// Generate various buffers objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_eao);

	// Bind the vertex array object
	glBindVertexArray(m_vao);

	// Bind the array buffer to setup the cube vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float)*m_vertex_attribute_array.size(),
		m_vertex_attribute_array.data(),
		GL_STATIC_DRAW);
	// Specify the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_vertex_stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Specify the normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*m_vertex_stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Bind the element array buffer to setup the triangle vertex indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int)*m_cube_face_indices.size(),
		m_cube_face_indices.data(),
		GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLCube::~GLCube() {
	m_vertex_attribute_array.clear();
}

GLCube::GLCube(const GLCube& aCopy):m_vao(aCopy.m_vao),
									m_vbo(aCopy.m_vbo), 
									m_eao(aCopy.m_eao), 
									m_vertex_attribute_array(aCopy.m_vertex_attribute_array),
									m_cube_face_indices(aCopy.m_cube_face_indices){

}

GLCube& GLCube::operator=(const GLCube& aRhs) {
	if (this != &aRhs) {
		m_vao = aRhs.m_vao;
		m_vbo = aRhs.m_vbo;
		m_eao = aRhs.m_eao;
		m_vertex_attribute_array = aRhs.m_vertex_attribute_array;
		m_cube_face_indices = aRhs.m_cube_face_indices;
	}
	return (*this);
}

void GLCube::draw() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_cube_face_indices.size(), GL_UNSIGNED_INT, 0);
}

void GLCube::fill_vertex_attribute_array() {


	float a_half_width = m_width * 0.5f;
	unsigned int index(0);
	// the vertices on the top face : v0, v1, v3, v1 , v2, v3
	// v0
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v1
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v3
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v1
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v2
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v3
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);

	// the vertices on the bottom face: v5, v4, v6, v4, v7, v6
	// v5
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v4
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v6
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v4
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v7
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v6
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);

	// the vertices on the front face: v3,v2,v7,v2,v6,v7
	// v3
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v2
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v7
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v2
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v6
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v7
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);

	// the vertices on the back face: v1,v0,v5,v0,v4,v5
	// v1
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v0
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v5
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v0
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v4
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v5
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);


	// the vertices on the left face: v0,v3,v4,v3,v7,v4
	// v0
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v3
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v4
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v3
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v7
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v4
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);

	// the vertices on the right face: v2,v1,v6,v1,v5,v6
	// v2
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v1
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v6
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v1
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(1.f);
	// v5
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	// v6
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(-a_half_width);
	m_vertex_attribute_array.emplace_back(a_half_width);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(0.f);
	m_vertex_attribute_array.emplace_back(1.f);
	m_vertex_attribute_array.emplace_back(0.f);
}

