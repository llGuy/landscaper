#pragma once

#include "ecs.h"
#include "../timer.h"
#include "graphics.h"
#include "../rock_data.h"
#include "../input_handler.h"
#include "basic_components.h"

/* rock throw component will be in charge of creating
 * new rocks and pushing them into the vector of entities
 * as well as creating new components and pushing those into the ECS */
template <> class component <struct rock_throw> : public icomponent
{
private:
	/* need to know when button M1 was pressed */
	input_handler * mouse;
	timer rock_throw_timer;
	i32 height_component_index;

	rock_stack *pending_rocks;
public:
	component(entity & subject, i32 index, input_handler & ih, rock_stack & pending_rocks)
		: mouse(&ih), pending_rocks(&pending_rocks), icomponent::icomponent(index)
	{
		rock_throw_timer.start();
		height_component_index = subject.get_component_index<height>();
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & data = entities[entity_index].get_data();
		auto & subject_height = ecs.get_component<height>(height_component_index).value.val;
		if (mouse->got_mouse_button(GLFW_MOUSE_BUTTON_1)) 
			/* make sure player cannot spam rocks */
			if (rock_throw_timer.elapsed() > 0.2f)
				create_rock(data, subject_height);
	}
private:
	auto create_rock(entity_data & player_data, f32 height) -> void
	{
		auto rock_start_position = player_data.pos + glm::vec3(0, height + 1.0f, 0);
		auto rock_start_direction = player_data.dir;

		/* initialize rock and push to pending rocks */
		rock_data new_rock
		{ 
			rock_start_position,
			rock_start_direction,
			entity_index
		};
		pending_rocks->push(new_rock);

		rock_throw_timer.reset();
	}
};