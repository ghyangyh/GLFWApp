#version 460 core

layout(location=0) in vec3 aVertexPos;
uniform mat4 aModelMat;
uniform mat4 aViewMat;
uniform mat4 aProjMat;


void main()
{
	gl_Position = aProjMat * aViewMat * aModelMat * vec4(aVertexPos, 1.0);
}