#pragma once

#include <GL/glew.h>

#include "timer.h"
#include "input_handler.h"
#include "renderable.h"

class game_state
{
public:
	game_state(void) = default;
	virtual ~game_state(void) = default;

	virtual auto render(timer & time) -> void = 0;
	virtual auto update(input_handler & ih, timer & time) -> game_state * = 0;

};