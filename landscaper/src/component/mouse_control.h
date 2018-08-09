#pragma once

#include "component.h"

struct mouse_control;

template <> struct component <mouse_control> : comp_base
{
	// pointer to input handler mouse map
	bool * mouse_buttons;
	

	auto update(void) -> void override 
	{ 

	}
};