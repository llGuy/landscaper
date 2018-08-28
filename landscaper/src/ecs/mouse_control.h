#pragma once

#include "ecs.h"
#include "../detail.h"
#include "../camera.h"
#include "../input_handler.h"

template <> class component <struct mouse_control> : public icomponent
{
private:
	input_handler * mouse;
	glm::vec2 previous_cursor;
public:
	component(entity &, i32 entity_index, input_handler & ih) 
		: mouse(&ih), previous_cursor(-0xFF), icomponent::icomponent(entity_index)
	{
	}
	component(void) = default;
	auto operator=(component &)->component & = default;
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		entity_data & ent = entities[entity_index].get_data();

		if (glm::all(glm::lessThan(previous_cursor, glm::vec2(-0xFE, -0xFE))))
			previous_cursor = mouse->cursor_position();
		else if (mouse->cursor_moved())
		{
			glm::vec2 difference = mouse->cursor_position() - previous_cursor;
			previous_cursor = mouse->cursor_position();
			ent.dir = look_fps(difference, ent.dir, td);
		}
	}
};