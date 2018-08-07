#include "glow_final_stage.h"
#include "render_pipeline.h"

auto glow_final_stage::create(int32_t width, int32_t height) -> void
{
	this->w = width;
	this->h = height;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT);

	shaders.create_shader(GL_VERTEX_SHADER, "shaders/blur/final_vsh.shader");
	shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/blur/final_fsh.shader");
	shaders.link_shaders("vertex_position", "texture_coords");
	shaders.get_uniform_locations("color_texture", "blur_texture");
	shaders.use();
	shaders.uniform_1i(0, 0);
	shaders.uniform_1i(1, 1);
}

auto glow_final_stage::bind(void) -> void
{
	main.bind();
}

auto glow_final_stage::render(quad_2D & quad, texture & color, texture & blur) -> void
{
	bind();
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);

	shaders.use();
	color.bind(GL_TEXTURE_2D, 0);
	blur.bind(GL_TEXTURE_2D, 1);
	render_model(quad, GL_TRIANGLE_STRIP);
}

auto glow_final_stage::create_texture(int32_t w, int32_t h) -> void
{
	out.create();
	out.bind(GL_TEXTURE_2D);
	out.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr, w, h);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

auto glow_final_stage::create_depth(int32_t w, int32_t h) -> void
{
	depth.create();
	depth.bind();
	depth.set_storage(GL_DEPTH_COMPONENT, w, h);
}