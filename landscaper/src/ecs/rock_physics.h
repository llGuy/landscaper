#pragma once

#include "ecs.h"
#include "../platform_handler.h"

template <> class component <struct rock_physics> : public icomponent
{
private:
	platform_handler * platforms;
public:
	component(entity & subject, i32 index, platform_handler & ph)
		: platforms(&ph), icomponent::icomponent(index)
	{
	}
	auto update(f32 td, std::vector<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & data = entities[entity_index].get_data();
		data.pos += data.dir * data.speed * td;
	}
};