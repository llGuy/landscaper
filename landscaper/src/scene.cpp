#include "scene.h"
#include "detail.h"
#include <glm/gtx/transform.hpp>

scene::scene(int32_t w, int32_t h, glm::vec2 const & cursor_pos, resource_handler & rh)
	: entities(cursor_pos), screen_res{ w, h }
{
	auto projection = glm::perspective(glm::radians(60.0f), (float)w / (float)h, 0.1f, 10000.0f);

	sky_box.create(rh, projection);
	pipeline.create(w, h, rh);
	platforms.create(rh, projection);
	water.create(rh, projection);
	guis.create(rh);

	guis.push(glm::vec2{ -0.5f, +0.5f }, 0.4f);

	glEnable(GL_DEPTH_TEST);
}

auto scene::render(timer & time) -> void
{
	prepare_water_renderer(time);

	pipeline.bind_default();
	auto view_matrix = entities.entity_cam().view_matrix();
	render_scene(view_matrix, detail::null_vector, time, true);

	pipeline.finalize_process();
}

auto scene::prepare_water_renderer(timer & time) -> void
{
	auto & cam = entities.entity_cam();
	glEnable(GL_CLIP_DISTANCE0);

	water.bind_refl();
	auto inverted_matrix = invert_matrix(cam);
	render_scene(inverted_matrix, water.refl_plane(), time, true);

	unbind_all_framebuffers(screen_res.w, screen_res.h);
	glDisable(GL_CLIP_DISTANCE0);
	glDisable(GL_BLEND);
}

auto scene::render_scene(glm::mat4 & view, glm::vec4 & plane, timer & t, bool render_water) -> void
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
}

auto scene::update(input_handler & ih, timer & time) -> game_state *
{
	entities.update(ih, time.elapsed());
	ih.cursor_moved() = false;

	return nullptr;
}

auto scene::render_debugging(void) -> void
{
}