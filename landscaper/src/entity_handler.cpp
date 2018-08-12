#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "entity_handler.h"
#include "render_pipeline.h"

entity_handler::entity_handler(void)
	: model(2.0f)
{
}

auto entity_handler::update(input_handler & ih, physics_handler & ph, f32 elapsed) -> void
{
	if (ih.got_key(GLFW_KEY_F1))
	{
		bound_entity = (bound_entity + 1) % num_entities;
		cam.bind_entity(players[bound_entity]);
	}

	key_system.update(0, elapsed);
	mouse_system.update(0, elapsed);
	display_system.update(0, elapsed);
	logging_system.update(0, elapsed);

	cam.update_view_matrix();
}

auto entity_handler::create(glm::mat4 & projection, resource_handler & rh, input_handler & ih) -> void
{
	model.create(rh);
	create_shaders(projection);
	create_main(ih, players[0]);
	create_local(ih, players[1]);

	bound_entity = 0;
	cam.bind_entity(players[bound_entity]);
}

auto entity_handler::create_shaders(glm::mat4 & projection) -> void
{
	shaders.create_shader(GL_VERTEX_SHADER, "shaders/entity/vsh.shader");
	shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/entity/fsh.shader");
	shaders.link_shaders("vertex_position");
	shaders.get_uniform_locations("projection_matrix", "view_matrix", "plane", "model_color", "model_matrix");
	shaders.use();
	shaders.uniform_mat4(&projection[0][0], 0);
}

auto entity_handler::prepare(glm::mat4 & view, glm::vec4 & plane) -> void
{
	glm::vec3 color { 1 };

	shaders.use();
	shaders.uniform_mat4(&view[0][0], 1);
	shaders.uniform_4f(&plane[0], 2);
	shaders.uniform_3f(&color[0], 3);
}

auto entity_handler::render(bool is_main_target) -> void
{
	graphics_system.update(0.0f, [](component<graphics> * c1, entity * main, bool is_main_target, f32 td)
	{
		if (c1->bound == main)
		{
			if (!is_main_target) c1->update(td);
		}
		else c1->update(td);
	}, &players[bound_entity], is_main_target, 0.0f);
}

auto entity_handler::create_main(input_handler & ih, entity & user) -> void
{
	add_component<graphics>(graphics_system, user, model, shaders);
	add_component<key_control>(key_system, user, ih);
	add_component<mouse_control>(mouse_system, user, ih);

	init_player(user);
}

auto entity_handler::create_local(input_handler & ih, entity & user) -> void
{
	add_component<graphics>(graphics_system, user, model, shaders);
	add_component<rotation_display>(display_system, user);

	user.pos = glm::vec3(10);
	user.dir = glm::vec3(1.001, 0.00001, 0.0001);
	user.vel = glm::vec3(20);
}

auto entity_handler::create_remote(void) -> entity
{
	return entity();
}

auto entity_handler::init_player(entity & ent) -> void
{
	ent.pos = glm::vec3(0, 10, 0);
	ent.dir = glm::vec3(1, 0.001, 0.001);
	ent.vel = glm::vec3(20);
}