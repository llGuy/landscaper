#pragma once

#include "ecs.h"
#include "../types.h"
#include "../detail.h"
#include "complex_key_control.h"
#include "../platform_handler.h"
#include <glm/gtx/string_cast.hpp>

template <> class component <struct player_physics> : public icomponent
{
private:
	static constexpr f32 gravity_at_sea = -37.5f;

	f32 ground_height;
	platform_handler * platforms;

	i32 flying_component_index;
	i32 max_walk_speed_component_index;
	i32 at_ground_height_component_index;
public:
	component(void) = default;
	component(entity & subject, i32 entity_index, platform_handler & ph)
		: platforms(&ph), icomponent::icomponent(entity_index)
	{
		flying_component_index = subject.get_component_index<is_flying>();
		max_walk_speed_component_index = subject.get_component_index<max_walk_speed>();
		at_ground_height_component_index = subject.get_component_index<is_at_ground_height>();
	}

	auto operator=(component &) -> component & = default;

	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;
		auto & flying             = ecs.get_component<is_flying>(flying_component_index).value.val;
		auto & ent_max_walk_speed = ecs.get_component<max_walk_speed>(max_walk_speed_component_index).value.val;
		auto & at_ground_height   = ecs.get_component<is_at_ground_height>(at_ground_height_component_index).value.val;
		entity_data & ent         = entities[entity_index].get_data();

		/* need to get ground height */
		ground_height = platforms->get_ground_height(ent.pos.x, ent.pos.z);

		ent.pos += ent.vel * td;

		if (!flying)
		{
			if (fequ(ground_height, ent.pos.y) || ground_height > ent.pos.y || fequ(ent.vel.y, 0.0f)) 
			{
				move_at_ground(ground_height, td, ent, at_ground_height, ent_max_walk_speed);
			}
			else
			{
				move_in_air(ground_height, td, ent, at_ground_height);
			}
		}
	}
private:
	auto move_at_ground(f32 ground_height, f32 td, entity_data & ent, bool & at_ground_height, f32 max_walk_speed) -> void
	{
		ent.pos.y = ground_height;
		ent.vel = glm::vec3(0);
		at_ground_height = true;

		/* increase the speed */
		if (ent.speed < max_walk_speed) ent.speed += max_walk_speed * 5.0f * td;
		else ent.speed = max_walk_speed;
	}

	auto move_in_air(f32 ground_height, f32 td, entity_data & ent, bool & at_ground_height) -> void
	{
		f32 vel_y = ent.vel.y;

		ent.vel.x *= (1.0f - td / 2.0f);
		ent.vel.z *= (1.0f - td / 2.0f);

		ent.vel.y = vel_y + gravity_at_sea * td;
		at_ground_height = false;

		ent.speed = 0;
	}
};