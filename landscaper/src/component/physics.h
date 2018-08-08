#pragma once

#include "component.h"
#include "key_control.h"

struct physics;

template <> struct component <physics> : comp_base
{
	static constexpr f32 gravity_at_sea = -10.5f;

	component<pos_3D> * pos;
	component<dir_3D> * dir;
	component<dir_3D> * mom;
	component<f32>    * vel;

	auto update(void) -> void override 
	{
		auto & ent_pos = pos->value;
		auto & ent_dir = dir->value;
		auto & ent_mom = mom->value;
		auto & ent_vel = vel->value;
	}
};