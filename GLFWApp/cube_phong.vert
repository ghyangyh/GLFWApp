#version 460 core

layout(location=0) in vec3 aCubeVertexPos;
layout(location=1) in vec3 aCubeVertexNormal;
layout(location=2) in vec2 aTextureCoords;

uniform mat4 aModelMat;
uniform mat4 aViewMat;
uniform mat4 aProjMat;

// output the cube vertex positoin in the eye space
out vec3 cube_vertex_eye_space;

// output the cube vertex normal in the eye space
out vec3 cube_normal_eye_space;
out vec2 texture_coords;

void main()
{
	// transform the cube vertex to the eye space
	cube_vertex_eye_space = vec3(aViewMat * aModelMat * vec4(aCubeVertexPos, 1.0));
	
	// transform the cube normal to the eye space
	cube_normal_eye_space = normalize(vec3(transpose(inverse(aViewMat * aModelMat)) * vec4(aCubeVertexNormal, 0.0)));

	// Model-View-Projection tranfomation, the position of each vertex
	// is transformed to a canonical space
	gl_Position = aProjMat * aViewMat * aModelMat * vec4(aCubeVertexPos, 1.0);
	
	// output the texture coordinates
	texture_coords = aTextureCoords;
}