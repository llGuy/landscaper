#include "log.h"
#include "application.h"

auto main(int32_t argc, char * argv[]) -> int32_t
{
	application app { 1700, 1000 };
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