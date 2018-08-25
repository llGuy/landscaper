#pragma once

#include "component.h"

struct shooting;
template <> struct component <shooting> : comp_base 
{
	entity * bound;



	auto update(f32 td) -> void override
	{

	}
};