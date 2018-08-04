#include "default_stage.h"

auto default_stage::create(int32_t w, int32_t h) -> void
{
	this->w = w;
	this->h = h;

	main.create(w, h);
	main.bind();

	create_texture(w, h);
	create_depth(w, h);

	main.attach(out, GL_COLOR_ATTACHMENT0, 0);
	main.attach(depth, GL_DEPTH_ATTACHMENT);
}

auto default_stage::bind(void) -> void
{
	main.bind();
}

auto default_stage::render(quad_2D & quad, texture & prev) -> void
{
	/*the rendering of the actual scene will happen in the game state
	  class, it will bind to this  */
}

auto default_stage::create_texture(int32_t w, int32_t h) -> void
{
	out.create();
	out.bind(GL_TEXTURE_2D);
	out.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr, w, h);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	out.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

auto default_stage::create_depth(int32_t w, int32_t h) -> void
{
	depth.create();
	depth.bind();
	depth.set_storage(GL_DEPTH_COMPONENT, w, h);
}