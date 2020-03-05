#version 460 core

uniform mat4 aViewMat;

in vec3 cube_vertex_eye_space, cube_normal_eye_space;

// Implementation of the Phong shading model 

// specify a fixed-position point light source
vec3 light_position_world_space = vec3(-6.0, 6.0, 6.0);
vec3 light_specular = vec3(1.0, 1.0, 1.0); // white specular
vec3 light_diffuse = vec3(0.7, 0.7, 0.7); // dull white diffuse
vec3 light_ambient = vec3(0.2, 0.2, 0.2); // grey ambient

// specify fragment materials
vec3 ks = vec3(1.0, 1.0, 1.0); // fully reflect specular light
vec3 kd = vec3(1.0, 0.5, 0.0); // orange diffuse reflectance color
vec3 ka = vec3(1.0, 1.0, 1.0); //fully reflected ambient light
float specular_exponent = 100.0; // the specular shiness

out vec4 frag_color;

void main()
{
	// Calculate the ambient color
	vec3 ambient_color = light_ambient * ka;

	// Calculate the diffuse color
	// Compute the light position in the eye space
	vec3 light_position_eye_space = vec3(aViewMat * vec4(light_position_world_space, 1.0));
	
	// Compute the light direction
	vec3 light_direction = normalize(light_position_eye_space - cube_vertex_eye_space);
	
	// Compute the diffuse dot production
	float diffuse_dot_prod = max(dot(light_direction, cube_normal_eye_space), 0.0);

	// the final diffuse color
	vec3 diffuse_color = light_diffuse * kd * diffuse_dot_prod; 

	// Calculate the specular color
	// Commpute the reflected light direction in the eye space
	vec3 reflected_specular_light_dir = reflect(-light_direction, cube_normal_eye_space);
	
	// In the eye space, the viewer or the virtual camera is at the origin
	vec3 eye_dir = normalize(vec3(-cube_vertex_eye_space));
	
	// Compute the shiness term
	float specular_dot_prod = max(dot(reflected_specular_light_dir, eye_dir), 0.0);
	float shiness = pow(specular_dot_prod, specular_exponent);

	// the final specular color
	vec3 specular_color = light_specular * ks * shiness; 

	// The final fragment color
	frag_color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
}
