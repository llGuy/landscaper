#include "entity.h"
#include "detail.h"

entity::entity(glm::vec3 const & p, glm::vec3 const & d, float speed)
	: position(p), direction(d), speed(speed)
{
}

auto entity::move(movement m, float elapsed, float gravity) -> void
{
	using detail::up;

	glm::vec3 final_direction;
	glm::vec3 lateral_dir{ direction.x, 0, direction.z };

	switch (m)
	{
	case movement::forward: { final_direction = glm::normalize(lateral_dir); break; }
	case movement::back: { final_direction = glm::normalize(-lateral_dir); break; }

	case movement::up: { final_direction = up; break; }
	case movement::down: { final_direction = -up; break; }

	case movement::left: { final_direction = glm::normalize(-glm::cross(lateral_dir, up)); break; }
	case movement::right: { final_direction = glm::normalize(glm::cross(lateral_dir, up)); break; }
	}

	position += final_direction * elapsed * speed * 20.0f;
}