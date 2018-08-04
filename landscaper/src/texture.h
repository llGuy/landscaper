#pragma once

#include <iostream>
#include <GL/glew.h>

class texture
{
public:
	friend class framebuffer;

	virtual auto create(void) -> void;

	auto bind(GLenum target, uint32_t slot = -1) -> void;
	auto fill(GLenum target, GLenum internal_format, 
		GLenum format, GLenum type, void const * data, int32_t w, int32_t h) -> void;

	auto enable_mipmap(GLenum target) -> void;
	auto float_param(GLenum target, GLenum mode, float factor) -> void;
	auto int_param(GLenum target, GLenum mode, GLenum factor) -> void;
protected:
	uint32_t id;

	int32_t w, h;
};

struct realistic_texture
{
	texture color;
	texture normal_map;
};

extern auto unbind_all_textures(GLenum target) -> void;
extern auto create_color_texture(texture &, int32_t w, int32_t h, void * data) -> void;
extern auto create_depth_texture(texture & t, int32_t w, int32_t h) -> void;