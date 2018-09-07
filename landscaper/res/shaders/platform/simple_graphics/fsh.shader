#version 330 core

in vec3 world_position;
in vec3 to_light;
in vec3 light_vector;
in vec3 to_camera;
in vec3 normal;
in vec3 light_pos;

out vec4 final_color;

uniform vec3 camera_position;
uniform vec3 light_position;

const vec4 platform_color = vec4(0.2, 0.2, 0.2, 1);
const vec3 light_color = vec3(1);

vec3 calculate_diffuse(vec3 normal)
{
	float diffuse = clamp(dot(to_light, normal) * 0.5f, 0, 1);
	return vec3(diffuse, diffuse, diffuse);
}

float power(float a, int e)
{
	float o = a;
	for (int i = 0; i < e; ++i)
	{
		o *= a;
	}
	return o;
}

vec3 calculate_specular(vec3 normal)
{
	vec3 l_vector = normalize(light_position - world_position);
	vec3 reflected_light = -reflect(l_vector, normal);
	vec3 eye_vector = normalize(camera_position - world_position);
	float specularity = clamp(dot(reflected_light, eye_vector), 0, 1);

	return vec3(power(specularity, 10)) * light_color;
}

void main(void)
{
	vec3 diffuse = calculate_diffuse(normal);
	vec3 specular = calculate_specular(normal);

	final_color = platform_color + vec4(diffuse, 1.0) +vec4(specular, 1.0f);
}