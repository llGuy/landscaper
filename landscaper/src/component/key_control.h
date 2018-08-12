#pragma once

#include "../detail.h"
#include "component.h"
#include "../input_handler.h"

struct key_control;
template <> struct component <key_control> : comp_base
{
	// pointer to input handler 
	input_handler * inputs;
	entity * bound;
	component(input_handler & ih) : inputs(&ih)
	{
	}
	component(void) = default;
	auto update(f32 td) -> void override
	{  
		using detail::up;

		dir_3D lateral_dir = glm::normalize(dir_3D(bound->dir.x, 0, bound->dir.z));

		if (inputs->got_key(GLFW_KEY_W)) bound->pos += lateral_dir * bound->vel * td;
		if (inputs->got_key(GLFW_KEY_A)) bound->pos += -glm::normalize(glm::cross(lateral_dir, up)) * bound->vel * td;
		if (inputs->got_key(GLFW_KEY_S)) bound->pos += -lateral_dir * bound->vel * td;
		if (inputs->got_key(GLFW_KEY_D)) bound->pos += glm::normalize(glm::cross(lateral_dir, up)) * bound->vel * td;
		if (inputs->got_key(GLFW_KEY_LEFT_SHIFT)) bound->pos += -up * bound->vel * td;

		if (inputs->got_key(GLFW_KEY_SPACE))
		{
			bound->pos += up * td;
			bound->vel.y = 10.0f;
		}
	};
};