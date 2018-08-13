#pragma once

#include "program.h"
//#include "camera.h"
#include "cube.h"
#include "platform_handler.h"
#include "input_handler.h"
#include "physics.h"

// testing component system
#include "component/log.h"
#include "component/physics.h"
#include "component/graphics.h"
#include "component/component.h"
#include "component/key_control.h"
#include "component/terraforming.h"
#include "component/mouse_control.h"
#include "component/rotate_display.h"

using entity_model = cube;

class entity_handler
{
public:
	entity_handler(void);

	auto create(glm::mat4 & projection, resource_handler & rh, input_handler & ih, platform_handler & ph) -> void;
	auto prepare(glm::mat4 & view, glm::vec4 & plane) -> void;
	auto render(bool is_main_target) -> void;

	auto update(input_handler & ih, physics_handler & ph, f32 elapsed) -> void;
public:
	inline auto entity_cam(void) -> camera & { return cam; };
private:
	auto create_shaders(glm::mat4 & projection) -> void;

private:
	// component stuff
	auto create_main(input_handler & ih, platform_handler & ph, entity & user) -> void;
	auto create_local(input_handler & ih, platform_handler & ph, entity & user) -> void;
	auto create_remote(void) -> entity;
	auto init_player(entity & ent) -> void;
private:
	static constexpr u32 num_entities = 2;

	program shaders;
	entity_model model;

	std::array<entity, num_entities> players;
	camera cam;
	u32 bound_entity;

	comp_system<terraforming, num_entities> terraforming_system;
	comp_system<rotation_display, num_entities> display_system;
	comp_system<mouse_control, num_entities> mouse_system;
	comp_system<graphics, num_entities> graphics_system;
	comp_system<physics, num_entities> physics_system;
	comp_system<key_control, num_entities> key_system;
	comp_system<logging, num_entities> logging_system;
};