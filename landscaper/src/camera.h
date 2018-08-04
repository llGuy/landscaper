#pragma once

#include "entity.h"
#include <glm/glm.hpp>

// camera controls direction, 
// entity controls position
class camera : public entity
{
public:
	camera(glm::vec2 const & cursor_xy);

	auto bind(int32_t entity) -> void;
	auto look_at(glm::vec2 const & cursor, float elapsed, float sens) -> void;
	auto update_view_matrix(void)->glm::mat4 &;

	inline auto pos(void) -> glm::vec3 & { return position; };
	inline auto dir(void) -> glm::vec3 & { return direction; };
	inline auto cursor_pos(void) -> glm::vec2 & { return cursor_ref; };
	inline auto view_matrix(void) -> glm::mat4 { return camera_view_matrix; };
private:
	// -1 if not bound to entity
	int32_t bound_entity;
	glm::vec2 cursor_ref;
	glm::mat4 camera_view_matrix;
};

extern auto look_fps(glm::vec2 const & cursor_dif, 
	glm::vec3 const & direction, 
	float elapsed, 
	float sens) -> glm::vec3;
extern auto invert_matrix(camera & cam) -> glm::mat4;