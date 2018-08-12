#pragma once

#include "../types.h"
#include "component.h"
#include "key_control.h"
#include "../platform_handler.h"

struct physics;
template <> struct component <physics> : comp_base
{
	static constexpr f32 gravity_at_sea = -10.5f;

	f32 ground_height;
	platform_handler * platforms;
	entity * bound;
	component(void) = default;
	component(platform_handler & ph) : platforms(&ph) {}
	auto operator=(component &)->component & = default;
	auto update(f32) -> void override 
	{
		// need to get ground height
	}
};