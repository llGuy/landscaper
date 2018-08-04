#include "entity_handler.h"

entity_handler::entity_handler(glm::vec2 const & cur)
	: cam(cur)
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