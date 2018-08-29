#pragma once

#include "ecs.h"
#include "../detail.h"
#include "../input_handler.h"
#include "basic_components.h"

template <> class component <struct complex_key_control> : public icomponent
{
private:
	/* pointer to input handler */
	input_handler * inputs;
	i32 flying_component_index;
	i32 at_ground_height_component_index;
public:
	component(entity & subject, i32 entity_index, input_handler & ih)
		: inputs(&ih), icomponent::icomponent(entity_index)
	{
		flying_component_index = subject.get_component_index<is_flying>();
		at_ground_height_component_index = subject.get_component_index<is_at_ground_height>();
	}
	component(void) = default;
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::up;

		entity_data & ent = entities[entity_index].get_data();
		auto & flying = ecs.get_component<is_flying>(flying_component_index).value.val;
		auto & at_ground_height = ecs.get_component<is_at_ground_height>(at_ground_height_component_index).value.val;

		if (flying)
		{
			ent.vel = glm::vec3(0);
			ent.speed = 50.0f;
			at_ground_height = false;
		}

		if (inputs->got_key(GLFW_KEY_P)) flying ^= true;

		if (at_ground_height || flying)
		{
			dir_3D lateral_dir = glm::normalize(dir_3D(ent.dir.x, 0, ent.dir.z));

			/* lateral movement */
			u32 count = 0;
			if (inputs->got_key(GLFW_KEY_W)) move(lateral_dir, ent.vel, td, count, ent);
			if (inputs->got_key(GLFW_KEY_A)) move(-glm::normalize(glm::cross(lateral_dir, up)), ent.vel, td, count, ent);
			if (inputs->got_key(GLFW_KEY_S)) move(-lateral_dir, ent.vel, td, count, ent);
			if (inputs->got_key(GLFW_KEY_D)) move(glm::normalize(glm::cross(lateral_dir, up)), ent.vel, td, count, ent);

			if (count == 0) ent.speed = 0;

			if (inputs->got_key(GLFW_KEY_LEFT_SHIFT)) move(-up, ent.vel, td, count, ent);

			if (inputs->got_key(GLFW_KEY_SPACE))
			{
				ent.vel.y = 12.0f;
				ent.pos += ent.vel * td;
			}
		}
	};

private:
	auto move(glm::vec3 const & dir, glm::vec3 & vel, f32 td, u32 & counter, entity_data const & data) -> void
	{
		vel += dir * data.speed;
		++counter;
	}
};