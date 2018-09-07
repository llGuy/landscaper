#version 330 core

out vec4 final_color;
in vec4 clip_space;
in vec2 texture_coords;
in vec3 to_camera;
in vec3 from_light_vector;
in vec4 world_position;

uniform sampler2D reflection_texture;
uniform sampler2D dudv_map;
uniform sampler2D normal_map;
uniform sampler2D depth_texture;
uniform vec3 camera_position;
uniform float move_factor;

//const vec3 light_color = vec3(1, 0.5, 0) * 0.7;
const vec3 light_color = vec3(1);
const float wave_strength = 0.02f;
const float shine_damper = 150.0f;
const float reflectivity = 0.4;

const float near = 0.001f;
const float far = 1000.0f;
const float density = 0.001f;
const float gradient = 1.5f;
const vec4 sky_color = vec4(0.4, 0.4 ,0.4, 1.0);

float calculate_visibility(vec4 relative_to_camera)
{
	float distance = length(relative_to_camera.xyz);
	float visib = exp(-pow((distance * density), gradient));
	visib = clamp(visib, 0.0, 1.0);
	return visib;
}

float convert_depth(float raw)
{
	return 2.0f * near * far / (far + near - (2.0f * raw - 1.0f) * (far - near));
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

void main(void)
{
	vec2 ndc = clip_space.xy / clip_space.w / 2.0f + 0.5f;
	vec2 reflection_tex_coords = vec2(ndc.x, -ndc.y);

	vec2 distorted_tex_coords = texture(dudv_map, vec2(texture_coords.x - move_factor, texture_coords.y)).rg * 0.1f;
	distorted_tex_coords = texture_coords + vec2(distorted_tex_coords.x, distorted_tex_coords.y + move_factor);
	vec2 total = (texture(dudv_map, distorted_tex_coords).rg * 2.0f - 1.0f) * wave_strength;

	reflection_tex_coords += total;
	reflection_tex_coords.x = clamp(reflection_tex_coords.x, 0.001f, 0.999);
	reflection_tex_coords.y = clamp(reflection_tex_coords.y, -0.999, -0.001f);
	
	vec4 reflection_color = texture(reflection_texture, reflection_tex_coords);
	vec4 normal_color = texture(normal_map, total);
	vec3 normal = vec3(normal_color.r * 2.0f - 1.0f, normal_color.b, normal_color.g * 2.0f - 1.0f);

	vec3 reflected_light = reflect(normalize(from_light_vector), normal);
	float specular = clamp(dot(reflected_light, normalize(to_camera)), 0, 1);
	specular = pow(specular, shine_damper);
//		power(specular, 31);
	vec3 specular_highlights = light_color * specular * reflectivity;

	final_color = reflection_color;//  +vec4(specular_highlights, 0.0f);
	final_color = mix(final_color, vec4(0, 0.5, 0.5, 1.0), 0.2);
	final_color = mix(sky_color, final_color, calculate_visibility(vec4(camera_position, 1.0f) - world_position));
}