#include "gui.h"
#include "render_pipeline.h"
#include <glm/gtx/transform.hpp>

auto gui_handler::create(resource_handler & rh) -> void
{
	quad.create(rh);

	gui_program.create_shader(GL_VERTEX_SHADER, "shaders/gui_quad/vsh.shader");
	gui_program.create_shader(GL_FRAGMENT_SHADER, "shaders/gui_quad/fsh.shader");
	gui_program.link_shaders("vertex_position", "texture_coords");
	gui_program.get_uniform_locations("model_matrix");
}

auto gui_handler::push(glm::vec2 const & translation, float scale) -> void
{
	glm::vec3 trans3D{ translation.x, translation.y, 0 };
	guis.emplace_back(glm::translate(trans3D) * glm::scale(glm::vec3(scale)));
}

auto gui_handler::prepare(gui_slot slot) -> void
{
	auto & model_matrix = guis[slot];
	gui_program.use();
	gui_program.uniform_mat4(&model_matrix[0][0], 0);
}

auto gui_handler::render(void) -> void
{
	render_model(quad, GL_TRIANGLE_STRIP);
}