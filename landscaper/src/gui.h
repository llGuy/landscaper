#pragma once

#include "quad_2D.h"
#include "program.h"
#include "texture.h"
#include <glm/glm.hpp>

enum gui_slot : i32
{
	// gui handler has a slot for debugging
	DEBUG_0,
};

class gui_handler
{
public:
	auto create(resource_handler & rh) -> void;
	auto push(glm::vec2 const & translation, f32 scale) -> void;

	auto prepare(gui_slot slot) -> void;
	auto render(void) -> void;
private:
	gui_quad quad;
	program gui_program;

	glm::mat4 projection;
	std::vector<glm::mat4> guis;
};