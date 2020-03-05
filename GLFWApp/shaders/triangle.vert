#version 460 core

layout(location=0) in vec3 aVertexPos;
uniform mat4 aTransformMat;


void main()
{
	gl_Position = aTransformMat * vec4(aVertexPos, 1.0);
}