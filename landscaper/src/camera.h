#pragma once

#include "component/component.h"
#include "types.h"

#include <glm/glm.hpp>

class camera
{
public:
	camera(void) = default;

	auto bind_entity(entity & e) -> void;
	auto update_view_matrix(void) -> void;

	inline auto bound_entity(void) -> entity * { return bound; };
	inline auto matrix(void) -> glm::mat4 & { return view_matrix; }
	inline auto pos(void) -> glm::vec3 &    { return bound->pos; }
	inline auto dir(void) -> glm::vec3 &    { return bound->dir; }
private:
	glm::mat4 view_matrix;

	entity * bound;
};

extern auto look_fps(glm::vec2 const & cursor_dif, 
	glm::vec3 const & direction, 
	f32 elapsed) -> glm::vec3;

extern auto invert_matrix(camera & cam) -> glm::mat4;