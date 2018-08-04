#pragma once

#include "input_handler.h"
#include "camera.h"

class entity_handler
{
public:
	entity_handler(glm::vec2 const & cur);

	auto update(input_handler & ih, float elapsed) -> void;
public:
	inline auto entity_cam(void) -> camera & { return cam; };
private:
	camera cam;
};