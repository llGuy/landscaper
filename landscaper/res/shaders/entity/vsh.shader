#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec4 player_color;
layout(location = 2) in mat4 translation_matrix;
out vec4 color;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform vec4 plane;

void main(void)
{
	vec4 world_position = translation_matrix * vec4(vertex_position, 1.0f);
	gl_Position = projection_matrix * view_matrix * world_position;
	gl_ClipDistance[0] = dot(world_position, plane);
	color = player_color;
}