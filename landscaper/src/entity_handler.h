#pragma once

#include "program.h"
//#include "camera.h"
#include "cube.h"
#include "platform_handler.h"
#include "input_handler.h"
#include "physics.h"

#include "camera.h"

#include "ecs/ecs.h"

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
	auto create_ecs(void) -> void;
	auto create_display(entity & user, glm::vec3 const & pos, i32 index) -> void;
	auto create_main(entity & user, input_handler & ih, platform_handler & ph, i32 index) -> void;
	auto create_remote(void) -> entity;
	auto init_player(entity & ent) -> void;
private:
	static constexpr u32 num_entities = 2;

	program shaders;
	entity_model model;
	 
	std::vector<entity> entities;

	camera cam;
	u32 bound_entity;

	entity_cs ecs;
};