#pragma once

#include "ecs.h"
#include "../timer.h"
#include "../platform_handler.h"

template <> class component <struct rock_physics> : public icomponent
{
private:
	static constexpr f32 gravity_at_sea = -37.5f;
	platform_handler * platforms;
	bool in_water {false};
public:
	component(entity & subject, i32 index, platform_handler & ph)
		: platforms(&ph), icomponent::icomponent(index)
	{
		auto & data = subject.get_data();
		data.vel = data.dir * data.speed;
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;

		auto & data = entities[entity_index].get_data();

		if (in_water) sink(td, data, entities, ecs);
		else
		{
			data.pos += data.vel * td;
			data.vel.y = data.vel.y + gravity_at_sea * td;

			f32 ground_height = platforms->get_ground_height(data.pos.x, data.pos.z);

			if (fequ(ground_height, data.pos.y) || ground_height > data.pos.y || fequ(data.vel.y, 0.0f))
			{
				/* landed in water */
				if (fequ(ground_height, 0.0f))
				{
					data.vel = glm::vec3(0);
					in_water = true;
				}
				/* still on terrain */
				else bounce(td, data, ground_height);
			}
		}
	}
private:
	auto kill_rock(vec_dd<entity> & entities, entity_cs & ecs) -> void
	{
		i32 index = entity_index;
		ecs.remove(entities[index]);
		entities.remove(index);
	}
	auto bounce(f32 td, entity_data & data, f32 ground_height) -> void
	{
		auto normal = platforms->get_normal(data.pos.x, data.pos.z);
		/* the rock is outside of the platform */
		if (normal.has_value()) data.vel = glm::reflect(data.vel, normal.value());
		data.vel *= (0.7 - td / 2.0f);
		data.pos.y = ground_height + 0.01f;
	}
	auto sink(f32 td, entity_data & data, vec_dd<entity> & entities, entity_cs & ecs) -> void
	{
		data.pos.y -= td / 10.0f;
		/* kill rock */
		if (data.pos.y < -3.0f) kill_rock(entities, ecs);
	}
};