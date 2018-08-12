#pragma once

#include <glm/gtx/transform.hpp>
#include "../render_pipeline.h"
#include "../renderable.h"
#include "../program.h"
#include "../detail.h"
#include "component.h"

struct graphics;
template <> struct component <graphics> : comp_base 
{
	entity const * bound;
	renderable * model;
	program * entity_shaders;

	component(renderable & m, program & shaders) : model(&m), entity_shaders(&shaders)
	{
	}
	component(void) = default;
	auto operator=(component &) -> component & = default;
	auto update(f32) -> void override
	{
		using detail::fequ;

		// calculate angle that player is facing
		float y_axis_rot = fequ(bound->dir.x, 0.0f) ? 0.00001f : -atan(bound->dir.z / bound->dir.x);
		float z_axis_rot = fequ(bound->dir.y, 0.0f) ? 0.00001f : -atan(bound->dir.x / bound->dir.y);
		float x_axis_rot = fequ(bound->dir.y, 0.0f) ? 0.00001f : -atan(bound->dir.z / bound->dir.y);

		glm::mat4 rotation = glm::rotate(y_axis_rot, glm::vec3(0, 1, 0)) *
			glm::rotate(z_axis_rot, glm::vec3(0, 0, 1)) *
			glm::rotate(x_axis_rot, glm::vec3(1, 0, 0));
		glm::mat4 model_matrix = glm::translate(bound->pos) * rotation;
		entity_shaders->uniform_mat4(&model_matrix[0][0], 4);

		render_model(*model, GL_TRIANGLES);
	}
};