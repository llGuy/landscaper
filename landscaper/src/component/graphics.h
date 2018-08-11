#pragma once

#include "../render_pipeline.h"
#include "../renderable.h"
#include "component.h"

struct graphics {};
template <> struct component <graphics> : comp_base 
{
	entity * bound;
	renderable * model;

	component(renderable & m) : model(&m) 
	{
	}
	component(void) = default;
	auto operator=(component &) -> component & = default;
	auto update(f32) -> void
	{
		render_model(*model, GL_TRIANGLES);
	}
};