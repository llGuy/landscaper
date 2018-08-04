#pragma once

#include <GL/glew.h>
#include <stdint.h>

class texture;
class renderbuffer;

extern auto unbind_all_framebuffers(int32_t w, int32_t h) -> void;

class framebuffer
{
public:
	framebuffer(void) = default;

	auto create(int32_t width, int32_t height) -> uint32_t;
	auto destroy(void) -> void;
	auto bind(void) -> void;
	auto unbind(void) -> void;

	auto attach(texture & obj, uint32_t component, uint32_t level) -> void;
	auto attach(renderbuffer & rnd, uint32_t component) -> void;

	auto select_color_buffer(uint32_t point) -> void;
	auto framebuffer_status(void) -> bool;

	inline auto width(void)  -> int32_t { return w; };
	inline auto height(void) -> int32_t { return h; };
private:
	uint32_t id;

	int32_t w, h;
};