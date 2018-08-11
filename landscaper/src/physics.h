#pragma once

#include <glm/glm.hpp>

//#include "entity.h"
#include "types.h"

enum class action { forward, back, left, right, up, down };

class physics_handler
{
public:
	physics_handler(void);
	//auto move(entity & ent, action a, f32 td) -> void;
private:
	f32 gravity_at_sea;
};