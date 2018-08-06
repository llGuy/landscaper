#pragma once

#include "quad_3D.h"
#include "texture.h"
#include "program.h"
#include "framebuffer.h"
#include "renderbuffer.h"

#include <glm/glm.hpp>

class water_handler
{
public:
	water_handler(void);

	auto create(resource_handler &, glm::mat4 & projection) -> void;
	auto prepare(glm::mat4 & view, glm::vec3 & cam_pos, glm::vec3 & light, float elapsed) -> void;
	auto render(void) -> void;

	auto bind_refl(void) -> void;

	auto refl_plane(void) -> glm::vec4 &;

	// getters
	auto refl_texture(void)->texture &;
private:
	auto get_dudv_map(resource_handler & rh) -> void;
	auto get_normal_map(resource_handler & rh) -> void;
	auto create_shaders(glm::mat4 & projection) -> void;
	auto create_reflection_fbo(int32_t, int32_t) -> void;
	auto create_color_texture_attachment(texture & t, framebuffer & fbo, int32_t, int32_t) -> void;
	auto create_depth_texture_attachment(texture & t, framebuffer & fbo, int32_t, int32_t) -> void; 
	auto create_depth_buffer_attachment(renderbuffer & r, framebuffer & fbo, int32_t w, int32_t h) -> void;
private:
	static constexpr int32_t reflection_width = 320;
	static constexpr int32_t reflection_height = 180;

	framebuffer water_reflection_fbo;

	texture refl_color;
	renderbuffer refl_depth;

	program water_shaders;

	quad_3D water_quad;

	texture dudv_texture;
	texture normal_map_texture;

	glm::vec4 reflection_plane;
};