#include "application.h"
#include "scene.h"

application::application(i32 w, i32 h)
	: appl_window(w, h, "landscaper"), resources("")
{
}

auto application::init(void) -> void
{
	state = std::make_unique<scene>(appl_window.pixel_width(), 
		                            appl_window.pixel_height(),
		                            appl_window.cursor_position(),
		                            resources);
	time.start();
}

auto application::init_window(void) -> void
{
	appl_window.window_hint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	appl_window.window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	appl_window.window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
	appl_window.window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	appl_window.init();
	appl_window.launch_input_handler();
	appl_window.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto application::render(void) -> void
{
	state->render(time);
}

auto application::update(void) -> void
{
	appl_window.refresh();
	state->update(appl_window.user_inputs(), time);
	fps = 1.0f / time.elapsed();
	time.reset();
}

auto application::running(void) -> bool
{
	return appl_window.is_open();
}

auto application::destroy(void) -> void
{
	appl_window.destroy();
}