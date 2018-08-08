#pragma once

#include "entity.h"
#include "types.h"

#include <glm/glm.hpp>

// camera controls direction, 
// entity controls position
class camera : public entity
{
public:
	camera(glm::vec2 const & cursor_xy);

	auto bind(i32 entity) -> void;
	auto look_at(glm::vec2 const & cursor, f32 elapsed, f32 sens) -> void;
	auto update_view_matrix(void)->glm::mat4 &;

	inline auto pos(void) -> glm::vec3 & { return position; };
	inline auto dir(void) -> glm::vec3 & { return direction; };
	inline auto cursor_pos(void) -> glm::vec2 & { return cursor_ref; };
	inline auto view_matrix(void) -> glm::mat4 { return camera_view_matrix; };
private:
	// -1 if not bound to entity
	i32 bound_entity;
	glm::vec2 cursor_ref;
	glm::mat4 camera_view_matrix;
};

/*template <> struct component <camera> : comp_base
{
	component<pos_3D> * bound_pos;
	component<dir_3D> * bound_dir;
	glm::mat4 view_matrix;
	pos_2D cursor_ref;
	f32 * td;
	u32 id;

	auto update(void) -> void override
	{

	};
};*/

extern auto look_fps(glm::vec2 const & cursor_dif, 
	glm::vec3 const & direction, 
	f32 elapsed, 
	f32 sens) -> glm::vec3;
extern auto invert_matrix(camera & cam) -> glm::mat4;