#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
out vec2 texture_coords;
out vec3 world_position;
out vec3 light_vector;
out vec3 light_pos_tangent;
out vec3 to_camera;
out vec3 to_light;
out vec3 test;

in vec2 pass_texture_coords[];
in vec3 pass_world_position[];

uniform vec3 camera_position;
uniform vec3 light_position;

vec3 calculate_tangent(int i1, int i2, int i3)
{
	vec3 delta_pos1 = pass_world_position[i2] - pass_world_position[i1]; 
	vec3 delta_pos2 = pass_world_position[i3] - pass_world_position[i1];

	vec2 delta_uv1 = pass_texture_coords[i2] - pass_texture_coords[i1];
	vec2 delta_uv2 = pass_texture_coords[i3] - pass_texture_coords[i1];

	float r = 1 / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);

	return (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
}

vec3 calculate_normal(int i1, int i2, int i3)
{
	vec3 diff_world_pos1 = normalize(pass_world_position[i2] - pass_world_position[i1]);
	vec3 diff_world_pos2 = normalize(pass_world_position[i3] - pass_world_position[i1]);
	return normalize(cross(diff_world_pos2, diff_world_pos1));
}

void main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		texture_coords = pass_texture_coords[i];
		world_position = pass_world_position[i];


		vec3 tangent = normalize(calculate_tangent(i, (i + 1) % 3, (i + 2) % 3));
		vec3 normal = normalize(calculate_normal(i, (i + 1) % 3, (i + 2) % 3));
		vec3 bitangent = normalize(cross(tangent, normal));

		mat3 to_tangent_space = mat3(

			tangent.x, normal.x, bitangent.x,
			tangent.y, normal.y, bitangent.y,
			tangent.z, normal.z, bitangent.z

		);

		light_pos_tangent = to_tangent_space * light_position;
		light_vector = to_tangent_space * (world_position - light_position);
		to_camera = to_tangent_space * (camera_position - world_position);
		to_light = to_tangent_space * normalize(light_position - world_position);
		test = bitangent;

		gl_ClipDistance[0] = gl_in[0].gl_ClipDistance[0];

		EmitVertex();
	}
	EndPrimitive();
}