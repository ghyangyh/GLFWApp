#version 460 core

layout(location=0) in vec3 aVertexPos;
layout(location=1) in vec3 aVertexColor;
uniform mat4 aModelMat;
uniform mat4 aViewMat;
uniform mat4 aProjMat;
out vec4 aInputColour;

void main()
{
	gl_Position = aProjMat * aViewMat * aModelMat * vec4(aVertexPos, 1.0);
	aInputColour = vec4(aVertexColor, 1.0);
}