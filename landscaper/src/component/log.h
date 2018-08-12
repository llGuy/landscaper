#pragma once

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "component.h"
#include "../detail.h"

struct logging;
template <> struct component <logging> : comp_base
{
	u32 count { 0 };

	entity const * bound;

	auto update(float f32) -> void override
	{
		count = (count + 1) % 200;

		if (count == 0)
		{
			using detail::fequ;
			using detail::debug;

			// log entity
			printf("logging entity %d' s info:\n", bound->id);
			printf("position : %s\n", glm::to_string(bound->pos).c_str());
			printf("direction : %s\n", glm::to_string(bound->dir).c_str());
			std::cout << std::endl;
		}
	}
};