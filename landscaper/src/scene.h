#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "game_state.h"
#include "input_handler.h"
#include "resources.h"
#include "entity_handler.h"
#include "render_pipeline.h"
#include "platform_handler.h"
#include "resolution.h"
#include "lighting.h"
#include "physics.h"
#include "water.h"
#include "sky.h"
#include "gui.h"

class scene : public game_state
{
public:
	scene(i32 w, i32 h, glm::vec2 const & cursor_pos, resource_handler & rh);

	auto render(timer & time) -> void override;
	auto update(input_handler & ih, timer & time) -> game_state * override;
private:
	auto render_scene(glm::mat4 & view_matrix, glm::vec4 & plane, timer & t, bool water = false) -> void;
	auto prepare_water_renderer(timer & t) -> void;
	auto render_debugging(void) -> void;
private:
	resolution screen_res;

	// gpu stuff
	render_pipeline pipeline;

	// gameplay elements
	sky sky_box;
	lighting light;
	gui_handler guis;
	water_handler water;
	entity_handler entities;
	platform_handler platforms;
	physics_handler physics;

	f32 previous_elapsed_time;
};