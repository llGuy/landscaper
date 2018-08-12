#pragma once

#include "../types.h"
#include "../detail.h"
#include "component.h"
#include "key_control.h"
#include "../platform_handler.h"

struct physics;
template <> struct component <physics> : comp_base
{
	static constexpr f32 gravity_at_sea = -15.5f;

	f32 ground_height;
	platform_handler * platforms;
	entity * bound;
	component(void) = default;
	component(platform_handler & ph) : platforms(&ph) {}
	auto operator=(component &)->component & = default;
	auto update(f32 td) -> void override 
	{
		using detail::fequ;

		/* need to get ground height */
		ground_height = platforms->get_ground_height(bound->pos.x, bound->pos.z);

		if (fequ(ground_height, bound->pos.y) || ground_height > bound->pos.y)
		{
			bound->pos.y = ground_height;
			bound->vel.y = 0;
		}
		else
		{
			bound->pos.y = bound->pos.y + bound->vel.y * td;
			bound->vel.y = bound->vel.y + gravity_at_sea * td;
		}
	}
};