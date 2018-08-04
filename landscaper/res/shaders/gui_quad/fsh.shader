#version 330 core

in vec2 out_texture_coords;
out vec4 final_color;

uniform sampler2D gui_texture;

void main(void)
{
	final_color = texture(gui_texture, out_texture_coords);
}