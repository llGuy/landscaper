#pragma once

#include <stack>
#include "types.h"
#include <glm/glm.hpp>

struct rock_data
{
	glm::vec3 start_position;
	glm::vec3 start_direction;
	i32 player;
};

using rock_stack = std::stack<rock_data, std::vector<rock_data>>;