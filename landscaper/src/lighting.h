#pragma once

#include <glm/glm.hpp>

class lighting
{
public:
	lighting(void) 
		: sun_pos(0, 1000.0f, -100000.0f)
	{
	}

	auto sun_position(void) -> glm::vec3 &
	{
		return sun_pos;
	}
private:
	glm::vec3 sun_pos;
};