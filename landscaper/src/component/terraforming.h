#pragma once

#include "component.h"
#include "../detail.h"
#include "../input_handler.h"
#include "../math/ray_trace.h"
#include "../platform_handler.h"

struct terraforming;
template <> struct component <terraforming> : comp_base 
{
	entity * bound;
	platform_handler * platforms;
	input_handler * mouse_input;

	component(input_handler & ih, platform_handler & ph) : mouse_input(&ih), platforms(&ph) {}
	component(void) = default;

	auto update (f32 td) -> void
	{
		if (mouse_input->got_mouse_button(GLFW_MOUSE_BUTTON_2))
		{
			/* terraform at the point the player is facing */

		}
	}
private:
	auto get_point_player_is_facing(void) -> glm::vec2 
	{
		using detail::fequ;

		entity_data & ent = bound->data;

		for (ray ray_cast(ent.pos, ent.dir, 0.25f, 6.0f); 
			ray_cast.distance_covered() < ray_cast.distance_max(); 
			ray_cast.extend())
		{
			auto ray_pos = ray_cast.current_position();
			f32 platform_height_at_ray = platforms->get_ground_height(ray_pos.x, ray_pos.z);
			if (fequ(platform_height_at_ray, ray_pos.y) || platform_height_at_ray > ray_pos.y)
			{
				f32 x = glm::round(ray_pos.x);
				f32 z = glm::round(ray_pos.z);

				//glm::ivec2 mesh_space = 
			}
		}
	}
};