#ifndef _GL_SPHERE_H
#define _GL_SPHERE_H

#include <vector>



class GLSphere {
public:
	GLSphere(float aRadius, int aVerticalSlices, int aHorizontalSlices);
	void draw();
protected:
	void fill_vertex_attribute_array(float aRadius, int aVerticalSlices, int aHorizontalSlices);
	void fill_triangle_face_indices(int aVerticalSlices, int aHorizontalSlices);
private:
	// each vertex has attributes: x, y, z, nx, ny, nz, s, t
	const int m_vertex_stride = 8;
	std::vector<float> m_vertex_atrribute_array;
	std::vector<unsigned int> m_triangle_face_indices;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_eao;
};


#endif

