#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "entity_handler.h"
#include "render_pipeline.h"
/* libraries for components */
#include "ecs/log.h"
#include "ecs/display.h"
#include "ecs/graphics.h"
#include "ecs/terraforming.h"
#include "ecs/mouse_control.h"
#include "ecs/player_physics.h"
#include "ecs/basic_components.h"
#include "ecs/basic_key_control.h"
#include "ecs/complex_key_control.h"

entity_handler::entity_handler(void)
	: model(2.0f)
{
}

auto entity_handler::update(input_handler & ih, physics_handler & ph, f32 elapsed) -> void
{
	ecs.update_except<graphics>(elapsed, entities);

	cam.update_view_matrix(ecs);
}

auto entity_handler::create(glm::mat4 & projection, resource_handler & rh, input_handler & ih, platform_handler & ph) -> void
{
	model.create(rh);
	create_shaders(projection);

	create_ecs();

	/* initialize entities */
	entities.push_back(entity());
	create_main(entities.back(), ih, ph, bound_entity);
	entities.push_back(entity());
	create_display(entities.back(), glm::vec3(40.0f, 10.0f, 40.0f), 1);

	/* initialize camera */
	bound_entity = 0;
	cam.bind_entity(entities[bound_entity]);
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
	ecs.update_only<graphics>(0, entities, [this, &is_main_target](i32 other) 
	{
		if (other == bound_entity)
		{
			if (!is_main_target) return true;
			else return false;
		}
		else return true;
	});
}

auto entity_handler::create_ecs(void) -> void
{
	/* most systems will start with a capacity of 32 because
	 * they are the systems that all entities need 
	 * e.g. height or, is_flying, max_walk_speed
	 */
	ecs.add_system<height>(32);
	ecs.add_system<logging>(32);
	ecs.add_system<is_flying>(32);
	ecs.add_system<mouse_control>(2);
	ecs.add_system<basic_key_control>(2);
	ecs.add_system<complex_key_control>(2);
	ecs.add_system<is_at_ground_height>(32);
	ecs.add_system<player_physics>(32);
	ecs.add_system<max_walk_speed>(32);
	ecs.add_system<terraforming>(32);
	ecs.add_system<graphics>(32);
	ecs.add_system<display>(2);
}

auto entity_handler::create_display(entity & user, glm::vec3 const & pos, i32 index) -> void
{
	entity_data & data = user.get_data();
	/* initialize data of the entity */
	data.pos = pos;
	data.dir = glm::vec3(1.001, 0.00001, 0.0001);

	ecs.add_component<display>(user, index);
	ecs.add_component<graphics>(user, index, model, shaders);
}

auto entity_handler::create_main(entity & user, input_handler & ih, platform_handler & ph, i32 index) -> void
{
	entity_data & data = user.get_data();
	/* initialize data of the entity */
	data.pos = glm::vec3(0, 10, 0);
	data.dir = glm::vec3(0.1, 0.1, 1.0f);
	data.vel = glm::vec3(0);
	data.speed = 0.0f;
	/* initialize the components of the entity */
	/* data initialization always comes first */
	ecs.add_component<height>(user, index, height{ 4.0f });
	ecs.add_component<is_flying>(user, index, is_flying{ false });
	ecs.add_component<max_walk_speed>(user, index, max_walk_speed{ 15.0f });
	ecs.add_component<is_at_ground_height>(user, index, is_at_ground_height{ false });

	ecs.add_component<mouse_control>(user, index, ih);
	ecs.add_component<complex_key_control>(user, index, ih);
	ecs.add_component<player_physics>(user, index, ph);
	ecs.add_component<terraforming>(user, index, ih, ph);
	ecs.add_component<graphics>(user, index, model, shaders);
}

auto entity_handler::create_remote(void) -> entity
{
	/* will need (to be implemented) network component */
	return entity();
}

auto entity_handler::init_player(entity & ent) -> void
{
	entity_data & data = ent.get_data();
	data.pos = glm::vec3(0, 10, 0);
	data.dir = glm::vec3(1, 0.001, 0.001);
	data.vel = glm::vec3(0.0f);
	data.speed = 0.0f;
	data.speed = 15.0f;
}