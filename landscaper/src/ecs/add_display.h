#pragma once

#include "ecs.h"
#include "../cube.h"
#include "display.h"
#include "../timer.h"
#include "graphics.h"
#include "basic_components.h"
#include "../input_handler.h"
#include "../batch_renderer.h"

using entity_model = cube;

template <> class component <struct add_display> : public icomponent
{
private:
	timer add_timer;
	program * shaders;
	batch_renderer_3D * renderer;
	input_handler * input;
public:
	component(entity & subject, i32 index, input_handler & ih, batch_renderer_3D & rnd)
		: input(&ih), icomponent::icomponent(index), renderer(&rnd)
	{
		add_timer.start();
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & host_entity = entities[entity_index].get_data();
		
		if (input->got_key(GLFW_KEY_LEFT_CONTROL) && add_timer.elapsed() > 0.2f)
		{
			i32 at_display = entities.add(entity());
			auto & new_entity = entities[at_display];
			auto & new_entity_data = new_entity.get_data();

			new_entity_data.pos = host_entity.pos;
			new_entity_data.dir = glm::vec3(1.001, 0.00001, 0.0001);
			new_entity_data.size = 1.0f;

			ecs.add_component<color>(new_entity, at_display, color{ glm::vec3(1) });
			ecs.add_component<display>(new_entity, at_display);
			ecs.add_component<graphics>(new_entity, at_display, *renderer);

			add_timer.reset();
		}
	}
};