#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
out vec2 texture_coords;
out vec3 world_position;
out vec3 light_vector;
out vec3 to_camera;

in vec2 pass_texture_coords[];
in vec3 pass_world_position[];

uniform vec3 camera_position;
uniform vec3 light_position;

void main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		texture_coords = pass_texture_coords[i];
		world_position = pass_world_position[i];
		light_vector = world_position - light_position;
		to_camera = camera_position - world_position;

		gl_ClipDistance[0] = gl_in[0].gl_ClipDistance[0];

		EmitVertex();
	}
	EndPrimitive();
}