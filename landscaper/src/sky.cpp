#include "sky.h"

sky::sky(void)
	: box(1000.0f)
{
}

auto sky::create(resource_handler & rh, glm::mat4 & projection) -> void
{
	box.create(rh);
	create_texture(rh);
	create_shaders(projection);
}

auto sky::create_texture(resource_handler & rh) -> void
{
	sky_texture.create();
	sky_texture.bind(GL_TEXTURE_CUBE_MAP, 0);

	std::array<std::string, 6> files { "right", "left", "up2", "down", "back", "front" };

	for (u32 i = 0; i < files.size(); ++i)
	{
		auto img = rh.load<image>("res/textures/sky/" + files[i] + ".png");
		sky_texture.fill(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_RGBA, 
			GL_RGBA, GL_UNSIGNED_BYTE, img.data, img.w, img.h);
	}
	sky_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	sky_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	sky_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	sky_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto sky::prepare(glm::mat4 & view, glm::vec4 & clip_plane) -> void
{
	sky_shaders.use();
	glm::mat4 view_no_translation (view);

	view_no_translation[3][0] = 0.0f;
	view_no_translation[3][1] = 0.0f;
	view_no_translation[3][2] = 0.0f;
	sky_shaders.uniform_mat4(&view_no_translation[0][0], 1);
	sky_shaders.uniform_4f(&clip_plane[0], 2);

	sky_texture.bind(GL_TEXTURE_CUBE_MAP, 0);
}

auto sky::create_shaders(glm::mat4 & projection) -> void
{
	sky_shaders.create_shader(GL_VERTEX_SHADER, "shaders/sky/vsh.shader");
	sky_shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/sky/fsh.shader");
	sky_shaders.link_shaders("vertex_position");
	sky_shaders.get_uniform_locations("projection_matrix", "view_matrix", "clip_plane");
	sky_shaders.use(); sky_shaders.uniform_mat4(&projection[0][0], 0);
}

auto sky::render(void) -> void
{
	render_model(box, GL_TRIANGLES);
}