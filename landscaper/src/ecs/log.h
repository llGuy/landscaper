#pragma once

#include "ecs.h"
#include <iostream>
#include "../detail.h"
#include <glm/gtx/string_cast.hpp>

template <> class component <struct logging> : public icomponent
{
public:
	u32 count{ 0 };
public:
	component(entity &, i32 entity_index)
		: icomponent::icomponent(entity_index)
	{
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		count = (count + 1) % 200;

		if (count == 0)
		{
			using detail::fequ;
			using detail::debug;

			auto & subject = entities[entity_index];
			printf("logging entity %d's info : \n", entity_index);
			printf("position : %s\n", glm::to_string(subject.get_data().pos).c_str());
			printf("direction : %s\n", glm::to_string(subject.get_data().dir).c_str());
			printf("entities size : %d \n", entities.amount());
			printf("entities cap : %d \n\n", entities.vec_size());
			std::cout << std::endl;
		}
	}
};