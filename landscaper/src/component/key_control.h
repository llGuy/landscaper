#pragma once

#include "component.h"

struct key_control;

template <> struct component <key_control> : comp_base
{
	// pointer to input handler key map
	bool * keys;

	component<pos_3D> * pos;
	component<dir_3D> * dir;
	component<f32>    * vel;

	auto update(void) -> void override {  };
};