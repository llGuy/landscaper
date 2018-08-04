#pragma once

#include "framebuffer.h"
#include "texture.h"
#include "program.h"
#include "quad_2D.h"
#include "renderbuffer.h"

class postproc_stage
{
public:
	postproc_stage(void) = default;

	virtual auto create(int32_t w, int32_t h) -> void = 0;
	virtual auto bind(void) -> void = 0;
	virtual auto render(quad_2D & quad, texture & prev) -> void = 0;

	inline auto output(void) -> texture & { return out; };
	inline auto fbo(void) -> framebuffer & { return main; }
protected:
	virtual auto create_texture(int32_t w, int32_t h) -> void {}
	virtual auto create_depth(int32_t w, int32_t h) -> void {}
protected:
	framebuffer main;
	texture out;
	renderbuffer depth;

	program shaders;

	int32_t w, h;
};