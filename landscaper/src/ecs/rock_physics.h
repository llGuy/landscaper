#pragma once

#include "ecs.h"
#include "../platform_handler.h"

template <> class component <struct rock_physics> : public icomponent
{
private:
	static constexpr f32 gravity_at_sea = -37.5f;
	platform_handler * platforms;
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
		data.pos += data.vel * td;

		f32 ground_height = platforms->get_ground_height(data.pos.x, data.pos.z);

		data.vel.y = data.vel.y + gravity_at_sea * td;

		if (fequ(ground_height, data.pos.y) || ground_height > data.pos.y || fequ(data.vel.y, 0.0f))
		{
			auto normal = platforms->get_normal(data.pos.x, data.pos.z);
			/* the rock is outside of the platform */
			if (normal.has_value()) data.vel = glm::reflect(data.vel, normal.value());
		}

		/* kill rock if under ocean */
		if (data.pos.y < -10.0f)
		{
			i32 index = entity_index;
			ecs.remove(entities[index]);
			entities.remove(index);
		}
	}
};