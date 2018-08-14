#pragma once

#include <optional>
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

	bool terraforming = false;
	glm::vec2 terraforming_point;

	component(input_handler & ih, platform_handler & ph) : mouse_input(&ih), platforms(&ph) {}
	component(void) = default;

	auto update (f32 td) -> void
	{
		if (mouse_input->got_mouse_button(GLFW_MOUSE_BUTTON_2))
		{
			/* terraform at the point the player is facing */
			if (!terraforming)
			{
				auto point = get_point_player_is_facing();
				if (point.has_value())
				{
					/* place terraforming mound */
					terraforming = true;
					terraforming_point = point.value();
				}
			}
			else
			{
				auto & platf = platforms->operator[](0);
				update_mound(td);
				platf.changed() = true;
			}
		}
		else
		{
			terraforming = false;
		}
	}
private:
	auto get_point_player_is_facing(void) -> std::optional<glm::vec2> 
	{
		using detail::fequ;

		entity_data & ent = bound->data;

		for (ray ray_cast(ent.pos + glm::vec3(0, ent.height, 0), ent.dir, 0.25f, 8.0f); 
			ray_cast.distance_covered() < ray_cast.distance_max(); 
			ray_cast.extend())
		{
			auto ray_pos = ray_cast.current_position();
			f32 platform_height_at_ray = platforms->get_ground_height(ray_pos.x, ray_pos.z);
			if (fequ(platform_height_at_ray, ray_pos.y) || platform_height_at_ray > ray_pos.y)
			{
				f32 x = glm::round(ray_pos.x);
				f32 z = glm::round(ray_pos.z);

				glm::vec3 world = glm::vec3(x, platform_height_at_ray, z);

				glm::vec2 mesh_space = platforms->operator[](0).get_platform_space_coord(world);

				if (platforms->operator[](0).is_within_mesh_space(mesh_space.x, mesh_space.y, default_mound_size))
					return std::make_optional<glm::vec2>(mesh_space);
				else return std::optional<glm::vec2>();
			}
		}
		return std::optional<glm::vec2>();
	}

	auto update_quarter(glm::ivec2 const & pos, i32 intensity_x, i32 intensity_z,
		bool update_center, bool update_radius, f32 td) -> void
	{
		auto & plat = platforms->operator[](0);
		auto & mound = platforms->mound_prot();

		for (u32 moundindex = 0; moundindex < mound.size(); ++moundindex)
		{
			auto mp = mound.at(moundindex, pos, intensity_x, intensity_z);
			/* check if mound point is on the platform */
			if (plat.is_on_platform_mesh_space(mp.coord.x, mp.coord.y))
			{
				bool update_vertex = true;
				if (!update_radius)
				{
					update_vertex &= !(mp.coord.x == pos.x || mp.coord.y == pos.y);
				}
				if (!update_center)
				{
					update_vertex &= !(mp.coord == pos);
				}
				if (update_vertex)
				{
					if (true)
					{
						f32 & vert_y = platforms->operator[](0).operator[](mp.coord);
						f32 newHeight = vert_y + mp.quotient * 15.0f;
						if (vert_y < newHeight) vert_y += mp.quotient * 15.0f* td;
					}
				}
			}
		}
	}

	auto update_mound(f32 td) -> void
	{
		update_quarter(terraforming_point, 1, 1, true, true, td);
		update_quarter(terraforming_point, 1, -1, false, false, td);
		update_quarter(terraforming_point, -1, 1, false, false, td);
		update_quarter(terraforming_point, -1, -1, false, true, td);
	}
};