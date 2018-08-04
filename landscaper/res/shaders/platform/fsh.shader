#version 330 core

in vec3 world_position;
in vec2 texture_coords;
in vec3 light_vector;
in vec3 to_camera;
out vec4 final_color;

uniform sampler2D grass_texture;
uniform sampler2D grass_normals;
uniform vec3 light_position;

void main(void)
{
	vec4 color = texture(grass_texture, texture_coords);
	vec3 raw_norm = texture(grass_normals, texture_coords).xyz;
	vec3 normal = vec3(raw_norm.r * 2.0f - 1.0f, raw_norm.b, raw_norm.g * 2.0f - 1.0f);

	vec3 reflected_light = reflect(normalize(light_vector), normal);
	float specular = dot(reflected_light, normalize(to_camera));
	specular = pow(specular, 50.0f);
	vec3 specular_highlights = vec3(0.5f) * specular * 0.2f;

	float diffuse = dot(normalize(light_position - world_position), normal) * 0.5f;
	vec4 brightness = vec4(diffuse, diffuse, diffuse, 1.0f);

	final_color = color + brightness * 0.8f;// +vec4(specular_highlights, 1.0f) * 0.0004f;
	final_color.a = 1;
}