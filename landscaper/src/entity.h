#pragma once

#include <glm/glm.hpp>
#include "types.h"

enum class movement { forward, left, back, right, up, down };

struct entity
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 momentum;
	f32 speed;
};