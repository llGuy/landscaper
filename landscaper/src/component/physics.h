#pragma once

#include "../types.h"
#include "../detail.h"
#include "component.h"
#include "key_control.h"
#include "../platform_handler.h"
#include <glm/gtx/string_cast.hpp>

struct physics;
template <> struct component <physics> : comp_base
{
	static constexpr f32 gravity_at_sea = -37.5f;

	f32 ground_height;
	platform_handler * platforms;
	entity * bound;

	component(void) = default;
	component(platform_handler & ph) 
		: platforms(&ph) 
	{
	}
	auto operator=(component &)->component & = default;

	auto update(f32 td) -> void override
	{
		using detail::fequ;
		entity_data & ent = bound->data;

		/* need to get ground height */
		ground_height = platforms->get_ground_height(ent.pos.x, ent.pos.z);

		ent.pos += ent.vel * td;

		if (!ent.flying)
		{
			if (fequ(ground_height, ent.pos.y) || ground_height > ent.pos.y || fequ(ent.vel.y, 0.0f))
			{
				move_at_ground(ground_height, td);
			}
			else
			{
				move_in_air(ground_height, td);
			}
		}
	}
private:
	auto move_at_ground(f32 ground_height, f32 td) -> void
	{
		entity_data & ent = bound->data;

		ent.pos.y = ground_height;
		ent.vel = glm::vec3(0);
		ent.at_ground_height = true;

		/* increase the speed */
		if (ent.speed < ent.max_walk_speed) ent.speed += ent.max_walk_speed * 5.0f * td;
		else ent.speed = ent.max_walk_speed;
	}

	auto move_in_air(f32 ground_height, f32 td) -> void
	{
		entity_data & ent = bound->data;

		f32 vel_y = ent.vel.y;

		ent.vel.x *= (1.0f - td / 2.0f);
		ent.vel.z *= (1.0f - td / 2.0f);

		ent.vel.y = vel_y + gravity_at_sea * td;
		ent.at_ground_height = false;

		ent.speed = 0;
	}
};