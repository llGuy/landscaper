#pragma once

#include "ecs.h"
#include <optional>
#include "../detail.h"
#include "basic_components.h"
#include "../input_handler.h"
#include "../math/ray_trace.h"
#include "../platform_handler.h"

template <> class component <struct terraforming> : public icomponent
{
private:
	platform_handler * platforms;
	input_handler * mouse_input;

	i32 platform_being_modified { -1 };
	glm::vec2 terraforming_point;

	i32 height_component_index;
public:
	component(entity & subject, i32 index, input_handler & ih, platform_handler & ph) 
		: mouse_input(&ih), platforms(&ph), icomponent::icomponent(index)
	{
		height_component_index = subject.get_component_index<height>();
	}
	component(void) = default;

	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & data = entities[entity_index].get_data();
		auto entity_height = ecs.get_component<height>(height_component_index).value.val;

		if (mouse_input->got_mouse_button(GLFW_MOUSE_BUTTON_2))
		{
			/* terraform at the point the player is facing */
			if (platform_being_modified == -1)
			{
				get_point_player_is_facing(data, entity_height);
			}
			else
			{
				auto & platf = platforms->operator[](platform_being_modified);
				update_mound(td);
				platf.changed() = true;
			}
		}
		else
		{
			platform_being_modified = -1;
		}
	}
private:
	auto get_point_player_is_facing(entity_data & ent, f32 height) -> void
	{
		using detail::fequ;

		platform_being_modified = platforms->which_platform(ent.pos.x, ent.pos.z);

		if (platform_being_modified != -1)
		{
			for (ray ray_cast(ent.pos + glm::vec3(0, height, 0), ent.dir, 0.25f, 8.0f);
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

					glm::vec2 mesh_space = platforms->operator[](platform_being_modified).get_platform_space_coord(world);

					/* check if mound will overlap with the borders */
					if (platforms->operator[](platform_being_modified).is_within_mesh_space(mesh_space.x, mesh_space.y, default_mound_size))
					{
						terraforming_point = mesh_space;
					}
				}
			}
		}
	}

	auto update_quarter(glm::ivec2 const & pos, i32 intensity_x, i32 intensity_z,
		bool update_center, bool update_radius, f32 td) -> void
	{
		auto & plat = platforms->operator[](platform_being_modified);
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
						f32 & vert_y = platforms->operator[](platform_being_modified).operator[](mp.coord);
						f32 newHeight = vert_y + mp.quotient * 50.0f;
						if (vert_y < newHeight) vert_y += mp.quotient * 50.0f* td;
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