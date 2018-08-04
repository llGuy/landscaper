#pragma once

#include <glm/glm.hpp>

enum class movement { forward, left, back, right, up, down };

class entity
{
public:
	entity(glm::vec3 const & p, glm::vec3 const & d, float speed);

	virtual auto move(movement ent_movement, float elapsed, float gravity) -> void;
protected:
	glm::vec3 position;
	glm::vec3 direction;

	float speed;
};