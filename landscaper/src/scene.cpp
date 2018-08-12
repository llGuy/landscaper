#include "scene.h"
#include "detail.h"
#include <glm/gtx/transform.hpp>

#define VIEW_TEST glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 10, 0) + glm::vec3(1.0f, 0.01f, 0.01f), detail::up)

scene::scene(i32 w, i32 h, glm::vec2 const & cursor_pos, resource_handler & rh, input_handler & ih)
	: screen_res{ w, h }
{
	auto projection = glm::perspective(glm::radians(60.0f), (f32)w / (f32)h, 0.1f, 10000.0f);

	sky_box.create(rh, projection);
	pipeline.create(w, h, rh);
	platforms.create(rh, projection);
	water.create(rh, projection);
	guis.create(rh);
	entities.create(projection, rh, ih);

	guis.push(glm::vec2{ -0.5f, +0.5f }, 0.4f);

	glEnable(GL_DEPTH_TEST);
}

auto scene::render(timer & time) -> void
{
	prepare_water_renderer(time);

	pipeline.bind_default();
	auto view_matrix = entities.entity_cam().matrix();
	render_scene(view_matrix, detail::null_vector, time, true);

	pipeline.bind_glow();
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);
	entities.prepare(view_matrix, detail::null_vector);
	entities.render(true);

	pipeline.finalize_process();
}

auto scene::prepare_water_renderer(timer & time) -> void
{
	auto & cam = entities.entity_cam();
	glEnable(GL_CLIP_DISTANCE0);

	water.bind_refl();
	auto inverted_matrix = invert_matrix(cam);
	render_scene(inverted_matrix, water.refl_plane(), time, false);

	unbind_all_framebuffers(screen_res.w, screen_res.h);
	glDisable(GL_CLIP_DISTANCE0);
	glDisable(GL_BLEND);
}

auto scene::render_scene(glm::mat4 & view, glm::vec4 & plane, timer & t, bool is_main_target) -> void
{
	auto & cam = entities.entity_cam();

	glEnable(GL_DEPTH_TEST);

	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);

	platforms.prepare(cam.pos(), light.sun_position(), plane);
	platforms.render(view);

	water.prepare(view, entities.entity_cam().pos(), light.sun_position(), t.elapsed());
	water.render();

	sky_box.prepare(view, plane);
	sky_box.render();

	entities.prepare(view, plane);
	entities.render(is_main_target);
}

auto scene::update(input_handler & ih, timer & time) -> game_state *
{
	entities.update(ih, physics, time.elapsed());
	ih.cursor_moved() = false;

	return nullptr;
}

auto scene::render_debugging(void) -> void
{
}