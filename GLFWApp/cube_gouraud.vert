#version 460 core

layout(location=0) in vec3 aCubeVertexPos;
layout(location=1) in vec3 aCubeVertexNormal;

// The MVP matrices
uniform mat4 aModelMat;
uniform mat4 aViewMat;
uniform mat4 aProjMat;

///*** Implementation of the Gouraud Shading Model ***///
// specify a fixed-position point light source in world space
vec3 light_position = vec3(-6.0, 6.0, 6.0);
vec3 light_specular = vec3(1.0, 1.0, 1.0); // white specular
vec3 light_diffuse = vec3(0.7, 0.7, 0.7); // dull white diffuse
vec3 light_ambient = vec3(0.2, 0.2, 0.2); // grey ambient

// specify fragment materials
vec3 ks = vec3(1.0, 1.0, 1.0); // fully reflect specular light
vec3 kd = vec3(1.0, 0.5, 0.0); // orange diffuse reflectance color
vec3 ka = vec3(1.0, 1.0, 1.0); //fully reflected ambient light
float specular_exponent = 100.0; // the specular shiness

// The virtual camera position
uniform vec3 aCameraPos;

// The output color
out vec3 output_color;

void main()
{
	// Model-View-Projection tranfomation
	// The position of each vertex is transformed into a canonical/clip space
	gl_Position = aProjMat * aViewMat * aModelMat * vec4(aCubeVertexPos, 1.0);

	// Compute the ambient color
	vec3 ambient_color = light_ambient * ka;

	// Compute the diffuse color
	vec3 light_dir = normalize(light_position - aCubeVertexPos);
	float diffuse_dot_prod = max(dot(light_dir, aCubeVertexNormal), 0.0);
	vec3 diffuse_color = light_diffuse * kd * diffuse_dot_prod;

	// Compute the specular color
	vec3 reflect_dir = reflect(-light_dir, aCubeVertexNormal);
	vec3 eye_dir = normalize(aCameraPos - aCubeVertexPos);
	float specular_dot_prod = max(dot(reflect_dir, eye_dir), 0.0);
	float shiness = pow(specular_dot_prod, specular_exponent);
	vec3 specular_color = light_specular * ks * shiness;
	
	// Compute the final color
	output_color = ambient_color + diffuse_color + specular_color;
	
	
}