#include "log.h"
#include "application.h"

auto main(i32 argc, char * argv[]) -> i32
{
	application app { 1136, 640 };
	glfw_init();
	app.init_window();
	glew_init();

	app.init();

	while (app.running())
	{
		app.render();
		app.update();
	}

	app.destroy();
	glfw_terminate();

	logger::sub_log("final frame per second ", app.fps);

	std::cin.get();
	
	return 0;
}