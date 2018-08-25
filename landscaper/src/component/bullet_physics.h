#pragma once

#include "component.h"
#include "../platform_handler.h"

struct bullet_physics;
template <> struct component <bullet_physics> : comp_base 
{
	static constexpr f32 gravity_at_sea = -37.5f;

	entity * bound;
	platform_handler * platforms;

	component(void) = default;
	component(platform_handler & ph) 
		: platforms(&ph)
	{
	}
	auto update(f32 td) -> void override
	{
		using detail::fequ;
		entity_data & ent = bound->data;

		f32 ground_height = platforms->get_ground_height(ent.pos.x, ent.pos.z);

		ent.pos += ent.vel * td;


	}
private:
	auto move_in_air(f32 ground_height, f32 td) -> void
	{
		entity_data & ent = bound->data;

		f32 vel_y = ent.vel.y;

//		ent.vel.x *= (1.0f - td / 2.0f);
//		ent.vel.z *= (1.0f - td / 2.0f);

		ent.vel.y = vel_y + gravity_at_sea * td;
		ent.at_ground_height = false;
	}
};