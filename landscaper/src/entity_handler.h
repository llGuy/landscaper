#pragma once

#include "program.h"
#include "camera.h"
#include "cube.h"
#include "input_handler.h"

using entity_model = cube;

class entity_handler
{
public:
	entity_handler(glm::vec2 const & cur);

	auto create(glm::mat4 & projection, resource_handler & rh) -> void;
	auto prepare(glm::mat4 & view, glm::vec4 & plane) -> void;
	auto render(void) -> void;

	auto update(input_handler & ih, float elapsed) -> void;
public:
	inline auto entity_cam(void) -> camera & { return cam; };
private:
	auto create_shaders(glm::mat4 & projection) -> void;
private:
	camera cam;
	program shaders;
	entity_model model;
};