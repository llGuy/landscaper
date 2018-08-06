// platforms are normal-mapped

#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 pass_texture_coords;
out vec3 pass_world_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 neg_corner;
uniform float dimension;
uniform vec4 clip_plane;

void main(void)
{
	pass_world_position = vertex_position;
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1);

	pass_texture_coords = texture_coords * 4.0f;
//	pass_texture_coords = vec2( vertex_position.x - neg_corner.x, vertex_position.z - neg_corner.z ) / dimension * 4.0f;

	gl_ClipDistance[0] = dot(vec4(pass_world_position, 1.0f), clip_plane);
}