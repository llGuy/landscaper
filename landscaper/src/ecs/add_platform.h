#pragma once

#include "ecs.h"
#include "../timer.h"
#include "../input_handler.h"
#include "../platform_handler.h"

template <> class component <struct add_platform> : public icomponent
{
private:
	timer adding_timer;
	input_handler * input;
	platform_handler * platforms;
public:
	component(entity & subject, i32 index, input_handler & ih, platform_handler & ph) 
		: platforms(&ph), input(&ih), icomponent::icomponent(index)
	{
		adding_timer.start();
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & data = entities[entity_index].get_data();
		if (input->got_mouse_button(GLFW_MOUSE_BUTTON_3))
		{
			if (adding_timer.elapsed() > 3.0f)
			{

				adding_timer.reset();
			}
		}
	}
};