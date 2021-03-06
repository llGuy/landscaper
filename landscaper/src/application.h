#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "game_state.h"
#include "window.h"
#include "resources.h"
#include "input_handler.h"
#include "timer.h"
#include "types.h"

class application
{
public:
	application(i32 w, i32 h);

	auto init(void) -> void;
	auto init_window(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;
	auto destroy(void) -> void;
private:
	window appl_window;
	std::unique_ptr<game_state> state; 
	resource_handler resources;
	timer time;

	// debugging
public:
	f32 fps;
};