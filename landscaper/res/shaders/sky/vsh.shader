#version 330 core

layout(location = 0) in vec3 vertex_position;
out vec3 texture_coords;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec4 clip_plane;

void main(void)
{
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position ,1);
	texture_coords = vertex_position;

	gl_ClipDistance[0] = dot(vec4(vertex_position, 1.0f), clip_plane);
}