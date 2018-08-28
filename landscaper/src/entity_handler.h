#pragma once

#include "cube.h"
#include "camera.h"
#include "vec_dd.h"
#include "ecs/ecs.h"
#include "program.h"
#include "physics.h"
#include "rock_data.h"
#include "input_handler.h"
#include "platform_handler.h"

using entity_model = cube;

class entity_handler
{
public:
	entity_handler(void);

	auto create(glm::mat4 & projection, resource_handler & rh, input_handler & ih, platform_handler & ph) -> void;
	auto prepare(glm::mat4 & view, glm::vec4 & plane) -> void;
	auto render(bool is_main_target) -> void;

	auto update(input_handler & ih, platform_handler & ph, f32 elapsed) -> void;
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
	auto create_rock(platform_handler & ph) -> void;
	auto init_player(entity & ent) -> void;
	auto bind_camera_to(i32 index) -> void;
private:
	static constexpr u32 num_entities = 2;

	program shaders;
	entity_model model;
	vec_dd<entity> entities;
	/* whenever M1 button is pressed, a new entity will be pushed
	 * to the pending rocks stack, and on every update function,
	 * the entity_handler will make sure to add the pending rocks to the
	 * vector of entities */
	rock_stack pending_rocks;
	camera cam;
	entity_cs ecs;
};