#pragma once

#include <GL/glew.h>
#include <iostream>

class renderbuffer
{
	friend class framebuffer;
public:
	renderbuffer(void) = default;

	auto create(void) -> uint32_t;
	auto bind(void) -> void;
	auto destroy(void) -> void;

	auto set_storage(GLenum component, uint32_t w, uint32_t h) -> void;
private:
	uint32_t id;
};

extern auto create_depth_buffer(renderbuffer & r, int32_t w, int32_t h) -> void;