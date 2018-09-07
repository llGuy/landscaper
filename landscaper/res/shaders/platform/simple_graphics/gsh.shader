#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
out vec3 world_position;
out vec3 light_vector;
out vec3 light_pos;
out vec3 to_camera;
out vec3 normal;
out vec3 to_light;

in vec3 pass_world_position[];

uniform vec3 camera_position;
uniform vec3 light_position;

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

		world_position = pass_world_position[i];

		normal = normalize(calculate_normal(0, 1, 2));
		light_pos = light_position;
		light_vector = world_position - light_position;
		to_camera = camera_position - world_position;
		to_light = normalize(light_position - world_position);


		gl_ClipDistance[0] = gl_in[0].gl_ClipDistance[0];

		EmitVertex();
	}
	EndPrimitive();
}