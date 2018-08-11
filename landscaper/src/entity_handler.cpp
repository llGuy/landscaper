#include <glm/gtx/transform.hpp>
#include "entity_handler.h"
#include "render_pipeline.h"

entity_handler::entity_handler(void)
	: model(2.0f)
{
}

auto entity_handler::update(input_handler & ih, physics_handler & ph, f32 elapsed) -> void
{
//	player.update(elapsed);
	for (u32 i = 1; i < entity::max_components; ++i)
		if(player.components[i])
			player.components[i]->update(elapsed);

	cam.update_view_matrix();
}

auto entity_handler::create(glm::mat4 & projection, resource_handler & rh, input_handler & ih) -> void
{
	model.create(rh);
	create_shaders(projection);
	create_local(ih, player);

	cam.bind_entity(player);
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
	glm::mat4 model_matrix = glm::translate(glm::vec3(0, 10.0f, 0));

	shaders.use();
	shaders.uniform_mat4(&view[0][0], 1);
	shaders.uniform_4f(&plane[0], 2);
	shaders.uniform_3f(&color[0], 3);
	shaders.uniform_mat4(&model_matrix[0][0], 4);
}

auto entity_handler::render(void) -> void
{
//	player.components[0]->update(0);

	render_model(model, GL_TRIANGLES);
}

auto entity_handler::create_local(input_handler & ih, entity & user) -> void
{
	add_component<graphics>(graphics_system, user, model);
	add_component<key_control>(key_system, user, ih);
	add_component<mouse_control>(mouse_system, user, ih);
	init_player(user);
}
auto entity_handler::create_remote(void) -> entity
{
	// add network component
	return entity();
}

auto entity_handler::init_player(entity & ent) -> void
{
	ent.pos = glm::vec3(0, 10, 0);
	ent.dir = glm::vec3(1, 0.001, 0.001);
	ent.vel = glm::vec3(5, 5, 5);
}