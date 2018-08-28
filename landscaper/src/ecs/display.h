#pragma once

#include "ecs.h"
#include <glm/gtx/transform.hpp>

template <> class component <struct display> : public icomponent
{
public:
	component(entity & subject, i32 index)
		: icomponent::icomponent(index)
	{
	}

	auto update(f32 td, std::vector<entity> & entities, entity_cs &) -> void override
	{
		auto & ent = entities[entity_index].get_data();

		ent.dir = glm::normalize(glm::mat3(glm::rotate(td, glm::vec3(0, 1, 0))) * ent.dir);
	}
};