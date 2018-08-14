#pragma once

#include "../detail.h"
#include "component.h"
#include "../input_handler.h"

struct key_control;
template <> struct component <key_control> : comp_base
{
	// pointer to input handler 
	input_handler * inputs;
	entity * bound;
	component(input_handler & ih) : inputs(&ih)
	{
	}
	component(void) = default;
	auto update(f32 td) -> void override
	{  
		using detail::up;

		entity_data & ent = bound->data;

		if (ent.flying)
		{
			ent.vel = glm::vec3(0);
			ent.speed = 30.0f;
			ent.at_ground_height = false;
		}

		if (inputs->got_key(GLFW_KEY_P)) ent.flying ^= true;

		if (ent.at_ground_height || ent.flying)
		{
			dir_3D lateral_dir = glm::normalize(dir_3D(ent.dir.x, 0, ent.dir.z));

			/* lateral movement */
			u32 count = 0;
			if (inputs->got_key(GLFW_KEY_W)) move(lateral_dir, ent.vel, td, count);
			if (inputs->got_key(GLFW_KEY_A)) move(-glm::normalize(glm::cross(lateral_dir, up)), ent.vel, td, count);
			if (inputs->got_key(GLFW_KEY_S)) move(-lateral_dir, ent.vel, td, count);
			if (inputs->got_key(GLFW_KEY_D)) move(glm::normalize(glm::cross(lateral_dir, up)), ent.vel, td, count);

			if (count == 0) ent.speed = 0;

			if (inputs->got_key(GLFW_KEY_LEFT_SHIFT)) move(-up, ent.vel, td, count);

			if (inputs->got_key(GLFW_KEY_SPACE))
			{
				ent.vel.y = 12.0f;
				ent.pos += ent.vel * td;
			}
		}
	};

private:
	auto move(glm::vec3 const & dir, glm::vec3 & vel, f32 td, u32 & counter) -> void
	{
		vel += dir * bound->data.speed;
		++counter;
	}
};