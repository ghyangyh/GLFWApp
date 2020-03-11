#ifndef _GL_CUBE_H
#define _GL_CUBE_H

#include <vector>

/* A cube class for OpenGL.
*  The cube is located at the origin of its object space.
*  Each vertex of the cube will by defalut has 3 attributes:
*  Position: x, y, z
*  Normal: nx, ny, nz
*  Texture coordinates: s, t
*  All of these is packed into one dimensional array:
*	x1,y1,z1,nx1,nx2,nz2,s1,t1,...,xi,yi,zi,nxi,nzi,si,ti,...,x8,y8,z8,nx8,ny8,nz8,s8,t8
*/
class GLCube {
public:
	GLCube(float aCubeWidth);
	~GLCube();
	GLCube(const GLCube& aCopy);
	GLCube& operator=(const GLCube& aRhs);
public:
	void draw();
protected:
	void fill_vertex_attribute_array();
	//void setup_normals();
	//void setup_texture_coordinates();
private:
	float m_width;
	const int m_vertex_stride = 8; // the number of stored floats for each vertex's attributes
	const int m_array_size= 36 * m_vertex_stride; // total number of floats for all vertices
	std::vector<float> m_vertex_attribute_array; // store the cube's vertices' data
	std::vector<unsigned int> m_cube_face_indices; // store the cube faces' vertex indices
	unsigned int m_vao; // the OpenGL's vertex array object
	unsigned int m_vbo; // the OpenGL's vertex buffer object
	unsigned int m_eao; // the OpenGL's element array object
};


#endif
