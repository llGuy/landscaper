#version 330 core

in vec2 blur_texture_coords[11];
out vec4 final_color;

uniform sampler2D original_texture;

void main(void)
{
	final_color = vec4(0.0);
	final_color += texture(original_texture, blur_texture_coords[0]) * 0.0093;
	final_color += texture(original_texture, blur_texture_coords[1]) * 0.028002;
	final_color += texture(original_texture, blur_texture_coords[2]) * 0.065984;
	final_color += texture(original_texture, blur_texture_coords[3]) * 0.121703;
	final_color += texture(original_texture, blur_texture_coords[4]) * 0.175713;
	final_color += texture(original_texture, blur_texture_coords[5]) * 0.198596;
	final_color += texture(original_texture, blur_texture_coords[6]) * 0.175713;
	final_color += texture(original_texture, blur_texture_coords[7]) * 0.121703;
	final_color += texture(original_texture, blur_texture_coords[8]) * 0.065984;
	final_color += texture(original_texture, blur_texture_coords[9]) * 0.028002;
	final_color += texture(original_texture, blur_texture_coords[10]) * 0.0093;
}