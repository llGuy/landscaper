#pragma once

#include "../detail.h"
#include "component.h"
#include <GLFW/glfw3.h>

struct key_control;

template <> struct component <key_control> : comp_base
{
	// pointer to input handler key map
	bool * keys;

	entity * bound;
	f32 td;

	auto set_data(f32 td) -> void { this->td = td; };

	auto update(void) -> void override
	{  
		using detail::up;

		dir_3D lateral_dir = glm::normalize(dir_3D(bound->dir.x, 0, bound->dir.z));

		if (keys[GLFW_KEY_W]) bound->pos += lateral_dir * bound->vel * td;
		if (keys[GLFW_KEY_A]) bound->pos += -glm::normalize(glm::cross(lateral_dir, up)) * bound->vel * td;
		if (keys[GLFW_KEY_S]) bound->pos += -lateral_dir * bound->vel * td;
		if (keys[GLFW_KEY_D]) bound->pos += glm::normalize(glm::cross(lateral_dir, up)) * bound->vel * td;
		if (keys[GLFW_KEY_SPACE]) bound->pos += up * bound->vel * td;
		if (keys[GLFW_KEY_LEFT_SHIFT]) bound->pos += -up * bound->vel * td;
	};
};