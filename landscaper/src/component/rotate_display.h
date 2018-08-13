#pragma once

#include <glm/gtx/transform.hpp>
#include "component.h"

struct rotation_display;
template <> struct component <rotation_display> : comp_base
{
	entity * bound;

	component(void) = default;

	auto update(f32 td) -> void override
	{
		entity_data & ent = bound->data;

		ent.dir = glm::normalize(glm::mat3(glm::rotate(td, glm::vec3(0, 1, 0))) * ent.dir);
	}
};