#pragma once

#include "../types.h"
#include "component.h"
#include "key_control.h"

struct physics;
template <> struct component <physics> : comp_base
{
	static constexpr f32 gravity_at_sea = -10.5f;

	

	auto update(f32) -> void override 
	{

	}
};