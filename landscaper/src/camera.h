#pragma once

//#include "component/component.h"
#include "ecs/ecs.h"
#include "types.h"

#include <glm/glm.hpp>

class camera
{
public:
	camera(void) = default;

	auto bind_entity(i32 index, std::vector<entity> & entities) -> void;
	auto update_view_matrix(std::vector<entity> & entities, entity_cs & ecs) -> void;

	inline auto bound_entity(void) -> i32 { return bound_entity_index; };
	inline auto matrix(void) -> glm::mat4 & { return view_matrix; }
	inline auto pos(void) -> glm::vec3 & { return position; }
	inline auto dir(void) -> glm::vec3 & { return direction; }
private:
	glm::mat4 view_matrix;

	glm::vec3 position;
	glm::vec3 direction;

	/* using indices because dealing with vectors */
	i32 bound_entity_index{ -1 };

	i32 height_component_index{ 0 };
};

extern auto look_fps(glm::vec2 const & cursor_dif, 
	glm::vec3 const & direction, 
	f32 elapsed) -> glm::vec3;

extern auto invert_matrix(camera & cam) -> glm::mat4;