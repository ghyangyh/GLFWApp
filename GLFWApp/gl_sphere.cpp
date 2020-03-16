#include "gl_sphere.h"

#include <GL/glew.h>

#include <cmath>
#include <iostream>
using namespace std;

const float PI = 3.1415926f;

GLSphere::GLSphere(float aRadius, 
					int aVerticalSlices, 
					int aHorizontalSlices): m_vao(0), 
											m_vbo(0),
											m_eao(0) {
	m_vertex_atrribute_array.reserve(aVerticalSlices * aHorizontalSlices + 2);
	m_triangle_face_indices.reserve(aVerticalSlices * aHorizontalSlices * 2);
	
	// Generate vertices
	fill_vertex_attribute_array(aRadius, aVerticalSlices, aHorizontalSlices);
	
	// Trianglulation
	fill_triangle_face_indices(aVerticalSlices, aHorizontalSlices);
	cout << "Sphere vertices: " << m_triangle_face_indices.size() << endl;
	cout << "Sphere triangles: " << m_triangle_face_indices.size() / 3 << endl;
	// Setup the VAO, VBO and EAO
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_eao);

	glBindVertexArray(m_vao);
	
	// Copy the vertex attribute data to OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float)*m_vertex_atrribute_array.size(), 
				m_vertex_atrribute_array.data(), 
				GL_STATIC_DRAW);
	// Specify the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_vertex_stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Specify the normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_vertex_stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Specify the texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, m_vertex_stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Copy the triangle face indces data to OpenGL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(unsigned int)*m_triangle_face_indices.size(), 
		m_triangle_face_indices.data(), 
		GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void GLSphere::fill_vertex_attribute_array(float aRadius, int aVerticalSlices, int aHorizontalSlices) {
	
	float d_theta = PI / (aVerticalSlices + 1);
	float d_phi = (2.f * PI) / aHorizontalSlices;

	// the samples in the vertical dirction contain two poles of the sphere
	int num_vertical_samples = aVerticalSlices + 2;
	int num_horizontal_samples = aHorizontalSlices;
	
	for (int i = 0; i <= num_vertical_samples; ++i) {
		for (int j = 0; j <= num_horizontal_samples; ++j) {
			// Compute x, y, z
			float x = aRadius * sin(d_theta*i)*cos(d_phi*j);
			float y = aRadius * cos(d_theta*i);
			float z = aRadius * sin(d_theta*i)*sin(d_phi*j);
			m_vertex_atrribute_array.emplace_back(x);
			m_vertex_atrribute_array.emplace_back(y);
			m_vertex_atrribute_array.emplace_back(z);

			// Compute nx, ny, nz
			m_vertex_atrribute_array.emplace_back(sin(d_theta*i)*cos(d_phi*j));
			m_vertex_atrribute_array.emplace_back(cos(d_theta*i));
			m_vertex_atrribute_array.emplace_back(sin(d_theta*i)*sin(d_phi*j));
			
			// Compute s, t
			/*float s = acos(y / aRadius) / PI;
			float t = (PI + atan2f(z, x)) / (2 * PI);*/
			float s = (d_phi * j) / (2 * PI);
			float t = (d_theta*i) / PI;
			//cout << "s = " << s << ", t = " << t << endl;
			m_vertex_atrribute_array.emplace_back(s);
			m_vertex_atrribute_array.emplace_back(t);

		}
	}
}

void GLSphere::fill_triangle_face_indices(int aVerticalSlices, int aHorizontalSlices) {
	// All the triangles is specified in CW order
	// the samples in the vertical dirction contain two poles of the sphere
	int num_vertical_samples = aVerticalSlices + 2;
	int num_horizontal_samples = aHorizontalSlices;
	// The strip containing the north pole
	for (int i = 1; i < num_horizontal_samples; ++i) {
		m_triangle_face_indices.emplace_back(0);
		m_triangle_face_indices.emplace_back(i);
		m_triangle_face_indices.emplace_back(i + 1);
	}
	m_triangle_face_indices.emplace_back(0);
	m_triangle_face_indices.emplace_back(num_horizontal_samples);
	m_triangle_face_indices.emplace_back(1);

	// Strips between north and south pole
	int current_circle_start_index, current_circle_stop_index, next_circle_start_index, next_circle_stop_index;
	for (int j = 1; j < aVerticalSlices; ++j) {
		current_circle_start_index = num_horizontal_samples * (j-1) + 1;
		current_circle_stop_index = current_circle_start_index + num_horizontal_samples - 1;
		next_circle_start_index = current_circle_start_index + num_horizontal_samples;
		next_circle_stop_index = next_circle_start_index + num_horizontal_samples - 1;

		// From the start to the stop: each time draw two tris
		for (int k = current_circle_start_index; k < current_circle_stop_index; ++k) {
			m_triangle_face_indices.emplace_back(k);
			m_triangle_face_indices.emplace_back(k+ num_horizontal_samples);
			m_triangle_face_indices.emplace_back(k + num_horizontal_samples+1);

			m_triangle_face_indices.emplace_back(k);
			m_triangle_face_indices.emplace_back(k + num_horizontal_samples + 1);
			m_triangle_face_indices.emplace_back(k + 1);

		}
		//From the stop to start
		m_triangle_face_indices.emplace_back(current_circle_stop_index);
		m_triangle_face_indices.emplace_back(next_circle_stop_index);
		m_triangle_face_indices.emplace_back(next_circle_start_index);

		m_triangle_face_indices.emplace_back(current_circle_stop_index);
		m_triangle_face_indices.emplace_back(next_circle_start_index);
		m_triangle_face_indices.emplace_back(current_circle_start_index);
	}

	// The strip containing the south pole
	int last_vertex_index = next_circle_stop_index + 1;
	for (int k = next_circle_start_index; k < next_circle_stop_index; ++k) {
		m_triangle_face_indices.emplace_back(last_vertex_index);
		m_triangle_face_indices.emplace_back(k+1);
		m_triangle_face_indices.emplace_back(k);
	}
	m_triangle_face_indices.emplace_back(next_circle_stop_index);
	m_triangle_face_indices.emplace_back(last_vertex_index);
	m_triangle_face_indices.emplace_back(next_circle_start_index);
}

void GLSphere::draw() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_triangle_face_indices.size(), GL_UNSIGNED_INT, 0);
}