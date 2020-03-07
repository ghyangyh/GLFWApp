#version 460 core

// The input color
in vec3 output_color;

out vec4 frag_color;

void main()
{
	frag_color = vec4(output_color, 1.0);
}
