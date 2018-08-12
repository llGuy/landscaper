#pragma once

#include "../detail.h"
#include "../camera.h"
#include "component.h"
#include "../input_handler.h"

struct mouse_control;
template <> struct component <mouse_control> : comp_base
{
	// pointer to input handler 
	input_handler * mouse;
	glm::vec2 previous_cursor;
	entity * bound;
	component(input_handler & ih) : mouse(&ih), previous_cursor(-0xFF)
	{
	}
	component(void) = default;
	auto operator=(component &)->component & = default;
	auto update(f32 td) -> void override 
	{
		if (glm::all(glm::lessThan(previous_cursor, glm::vec2(-0xFE, -0xFE))))
			previous_cursor = mouse->cursor_position();
		else if(mouse->cursor_moved())
		{
			glm::vec2 difference = mouse->cursor_position() - previous_cursor;
			previous_cursor = mouse->cursor_position();
			bound->dir = look_fps(difference, bound->dir, td);
		}
	}
};