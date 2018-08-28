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
		auto & data = entities[entity_index].get_data();
		data.pos += data.vel * td;

		data.vel.y = data.vel.y + gravity_at_sea * td;
	}
};