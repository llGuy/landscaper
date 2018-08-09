#include <glm/gtx/transform.hpp>
#include "entity_handler.h"
#include "render_pipeline.h"

entity_handler::entity_handler(glm::vec2 const & cur)
	: cam(cur), model(2.0f)
{
}

auto entity_handler::update(input_handler & ih, physics_handler & ph, f32 elapsed) -> void
{
	entity & bound = cam;

	if (ih.got_key(GLFW_KEY_W)) ph.move(bound, action::forward, elapsed);
	if (ih.got_key(GLFW_KEY_A)) ph.move(bound, action::left, elapsed);
	if (ih.got_key(GLFW_KEY_S)) ph.move(bound, action::back, elapsed);
	if (ih.got_key(GLFW_KEY_D)) ph.move(bound, action::right, elapsed);
	if (ih.got_key(GLFW_KEY_SPACE)) ph.move(bound, action::up, elapsed);
	if (ih.got_key(GLFW_KEY_LEFT_SHIFT)) ph.move(bound, action::down, elapsed);

	if (ih.cursor_moved()) 
		cam.look_at(ih.cursor_position(), elapsed, 0.02f);
	cam.update_view_matrix();
}

auto entity_handler::create(glm::mat4 & projection, resource_handler & rh) -> void
{
	model.create(rh);
	create_shaders(projection);

	// initialize component system
	add_component(component<key_control>(), key_system, player);
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
	render_model(model, GL_TRIANGLES);
}