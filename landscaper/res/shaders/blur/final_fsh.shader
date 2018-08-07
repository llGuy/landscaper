#version 330 core

in vec2 pass_texture_coords;
out vec4 final_color;

uniform sampler2D color_texture;
uniform sampler2D blur_texture;

void main(void)
{
	final_color = texture(color_texture, pass_texture_coords) + texture(blur_texture, pass_texture_coords);
}