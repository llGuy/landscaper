#include "physics.h"
#include "detail.h"

physics_handler::physics_handler(void)
	: gravity_at_sea(-10.5f)
{
}

//auto physics_handler::move(entity & ent, action a, f32 td) -> void
//{
	/*using detail::up;

	ent.momentum = glm::vec3(0.0f);
	glm::vec3 lateral_dir = { ent.direction.x, 0, ent.direction.z };

	switch (a)
	{
	case action::forward: { ent.momentum = glm::normalize(lateral_dir); break; }
	case action::back: { ent.momentum = glm::normalize(-lateral_dir); break; }

	case action::up: { ent.momentum = up; break; }
	case action::down: { ent.momentum = -up; break; }

	case action::left: { ent.momentum = glm::normalize(-glm::cross(lateral_dir, up)); break; }
	case action::right: { ent.momentum = glm::normalize(glm::cross(lateral_dir, up)); break; }
	}

	ent.position += ent.momentum * ent.speed * 20.0f * td;*/
//}