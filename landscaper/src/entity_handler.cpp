#include <glm/gtx/transform.hpp>
#include "entity_handler.h"
#include "render_pipeline.h"

entity_handler::entity_handler(glm::vec2 const & cur)
	: cam(cur), model(4.0f)
{
}

auto entity_handler::update(input_handler & ih, float elapsed) -> void
{
	entity & bound = cam;

	if (ih.got_key(GLFW_KEY_W)) bound.move(movement::forward, elapsed, 0);
	if (ih.got_key(GLFW_KEY_A)) 
		bound.move(movement::left, elapsed, 0);
	if (ih.got_key(GLFW_KEY_S)) bound.move(movement::back, elapsed, 0);
	if (ih.got_key(GLFW_KEY_D)) bound.move(movement::right, elapsed, 0);
	if (ih.got_key(GLFW_KEY_SPACE)) bound.move(movement::up, elapsed, 0);
	if (ih.got_key(GLFW_KEY_LEFT_SHIFT)) bound.move(movement::down, elapsed, 0);

	if (ih.cursor_moved()) 
		cam.look_at(ih.cursor_position(), elapsed, 0.02f);
	cam.update_view_matrix();
}

auto entity_handler::create(glm::mat4 & projection, resource_handler & rh) -> void
{
	model.create(rh);
	create_shaders(projection);
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
	glm::mat4 model_matrix = glm::translate(glm::vec3(-40.0f, 10.0f, -40.0f));

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