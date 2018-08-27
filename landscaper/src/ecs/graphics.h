#pragma once

#include "ecs.h"
#include "../detail.h"
#include "../program.h"
#include "../renderable.h"
#include "basic_components.h"
#include "../render_pipeline.h"
#include <glm/gtx/transform.hpp>

template <> struct component <struct graphics> : public icomponent
{
	renderable * model;
	program * entity_shaders;

	i32 height_component_index;

	component(entity & subject, i32 index, renderable & m, program & shaders) 
		: model(&m), entity_shaders(&shaders), icomponent::icomponent(index)
	{
		height_component_index = subject.get_component_index<height>();
	}
	component(void) = default;

	auto operator=(component &)->component & = default;

	auto update(f32, std::vector<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;
		auto const & ent = entities[entity_index].get_data();
		auto entity_height = ecs.get_component<height>(height_component_index).value.val;

		// calculate angle that player is facing
		float y_axis_rot = fequ(ent.dir.x, 0.0f) ? 0.00001f : -atan(ent.dir.z / ent.dir.x);
		float z_axis_rot = fequ(ent.dir.y, 0.0f) ? 0.00001f : -atan(ent.dir.x / ent.dir.y);
		float x_axis_rot = fequ(ent.dir.y, 0.0f) ? 0.00001f : -atan(ent.dir.z / ent.dir.y);

		glm::vec3 translate_vec = ent.pos;
		translate_vec.y += entity_height;

		glm::mat4 rotation = glm::rotate(y_axis_rot, glm::vec3(0, 1, 0)) *
			glm::rotate(z_axis_rot, glm::vec3(0, 0, 1)) *
			glm::rotate(x_axis_rot, glm::vec3(1, 0, 0));
		glm::mat4 model_matrix = glm::translate(translate_vec) * rotation;
		entity_shaders->uniform_mat4(&model_matrix[0][0], 4);

		render_model(*model, GL_TRIANGLES);
	}
};