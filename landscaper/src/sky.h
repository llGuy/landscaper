#pragma once

#include "cube.h"
#include "program.h"
#include "texture.h"
#include "render_pipeline.h"
#include <glm/glm.hpp>

class sky
{
public:
	sky(void);

	auto create(resource_handler & rh, glm::mat4 & projection) -> void;
	auto prepare(glm::mat4 & view, glm::vec4 & clip_plane) -> void;
	auto render(void) -> void;

	auto vao(void) -> vertex_layout & { return box.vao(); };
	auto count(void) -> uint32_t { return 36; };
	auto index_buffer(void) -> buffer & { return **box.element_buffer(); };
private:
	auto create_texture(resource_handler & rh) -> void;
	auto create_shaders(glm::mat4 & projection) -> void;
private:
	cube box;
	texture sky_texture;
	program sky_shaders;
};