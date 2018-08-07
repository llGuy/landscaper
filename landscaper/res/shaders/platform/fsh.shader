#version 330 core

in vec3 world_position;
in vec2 texture_coords;

in vec3 to_light;
in vec3 light_vector;
in vec3 to_camera;
in vec3 test;
in vec3 face_normal;
in float angle_cos;
in vec3 light_pos_tangent;

out vec4 final_color;

uniform sampler2D grass_texture;
uniform sampler2D grass_normals;
uniform sampler2D dirt_texture;
uniform sampler2D dirt_normals;

uniform vec3 light_position;

const vec3 light_color = vec3(1, 0.5, 0) * 0.7;

vec3 calculate_diffuse(vec3 normal)
{
	float diffuse = clamp(dot(to_light, normal) * 0.5f, 0, 1);
	return vec3(diffuse, diffuse, diffuse);
}

vec3 calculate_specular(vec3 normal)
{
	vec3 reflected_light = reflect(normalize(light_vector), normal);
	float specular = dot(reflected_light, normalize(to_camera));
	specular = pow(specular, 30.0f);
	vec3 specular_highlights = light_color * specular;

	return specular_highlights;
}

//const vec3 grass_color = vec3(0.14, 0.56, 0.14);
//const vec3 dirt_color = vec3(0.45, 0.26, 0.24);

void main(void)
{
	vec4 grass_color = texture(grass_texture, texture_coords);
    vec3 grass_raw_norm = texture(grass_normals, texture_coords).xyz;

	vec4 dirt_color = texture(dirt_texture, texture_coords);
	vec3 dirt_raw_norm = texture(dirt_normals, texture_coords).xyz;

	vec3 grass_normal = vec3(grass_raw_norm.r * 2.0f - 1.0f, grass_raw_norm.b, grass_raw_norm.g * 2.0f - 1.0f);
	vec3 dirt_normal = vec3(dirt_raw_norm.r * 2.0f - 1.0f, dirt_raw_norm.b, dirt_raw_norm.g * 2.0f - 1.0f);

	vec3 grass_diffuse = calculate_diffuse(grass_normal);
	vec3 grass_specular = calculate_specular(grass_normal);
	
	vec3 dirt_diffuse = calculate_diffuse(dirt_normal);
	vec3 dirt_specular = calculate_specular(dirt_normal) * 0.0004;

	vec4 diffuse = vec4(mix(dirt_diffuse, grass_diffuse, angle_cos), 1.0);
	vec4 specular = vec4(mix(dirt_specular, grass_specular, angle_cos), 1.0);

	//vec4 specular = vec4(calculate_specular(face_normal).xyz, 1.0f);
	//vec4 diffuse = vec4(calculate_specular(face_normal).xyz, 1.0f);

	final_color = mix(dirt_color, grass_color, angle_cos) + diffuse * 0.8f + specular;
}