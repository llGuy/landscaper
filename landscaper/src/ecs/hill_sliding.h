#pragma once

#include "ecs.h"
#include "../input_handler.h"
#include "../platform_handler.h"
#include <glm/gtx/string_cast.hpp>

template <> struct component <struct hill_sliding> : public icomponent
{
private:
	input_handler * keys;
	platform_handler * platforms;

	glm::vec3 towards_vector;

	f32 momentum;
	i32 max_walk_speed_component_index;
	i32 at_ground_height_component_index;
public:
	component(entity & subject, i32 index, input_handler & ih, platform_handler & ph)
		: keys(&ih), platforms(&ph), icomponent::icomponent(index), momentum(0.0f)
	{
		max_walk_speed_component_index = subject.get_component_index<max_walk_speed>();
		at_ground_height_component_index = subject.get_component_index<is_at_ground_height>();
	}

	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;
		auto & data = entities[entity_index].get_data();

		bool got = keys->got_key(GLFW_KEY_Z) || keys->got_key(GLFW_KEY_X) || keys->got_key(GLFW_KEY_C) || keys->got_key(GLFW_KEY_V) || keys->got_key(GLFW_KEY_E);

		if (keys->got_key(GLFW_KEY_LEFT_SHIFT) || got)
		{
			/* get the normal of the platform at the player */
			auto plat_normal_at_player = platforms->get_normal(data.pos.x, data.pos.z);
			if (plat_normal_at_player.has_value())
			{
				glm::vec3 normal = plat_normal_at_player.value();
				if (!fequ(normal.y, 1.0f))
				{
					/* get lateral movement direction */
					/* need to calculate down vector */
					momentum += 5.0f * td;
					auto lateral_direction = glm::normalize(glm::vec3(data.dir.x, 0, data.dir.z));

					auto next_possible_position = data.pos + lateral_direction * 0.06f;
					next_possible_position.y = platforms->get_ground_height(next_possible_position.x, next_possible_position.z);
					//auto towards_vector = glm::normalize(next_possible_position - data.pos);

					/* wrong maths */
					auto tangent = glm::normalize(glm::cross(data.dir, normal));
					towards_vector = glm::normalize(glm::cross(normal, tangent));

					if (keys->got_key(GLFW_KEY_E))
					{
						detail::do_nothing();

					}

					data.speed = ecs.get_component<max_walk_speed>(max_walk_speed_component_index).value.val * momentum;
					data.vel = towards_vector * data.speed;

					/*printf("%s\n%s\n%s\n%s\n\n", glm::to_string(data.dir).c_str(), glm::to_string(tangent).c_str(), glm::to_string(towards_vector).c_str(), glm::to_string(normal).c_str());

					if (keys->got_key(GLFW_KEY_Z))
					{
						data.vel = normal * data.speed;
					}
					else if (keys->got_key(GLFW_KEY_X))
					{
						data.vel = tangent * data.speed;
					}
					else if (keys->got_key(GLFW_KEY_C))
					{
						data.vel = towards_vector * data.speed;
					}
					else if (keys->got_key(GLFW_KEY_V))
					{
						data.vel = data.dir * data.speed;
					}*/
				}
			}
		}
		else
		{
			momentum = 0.0f;

		}
	}
};