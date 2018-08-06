#include "platform_handler.h"
#include "render_pipeline.h"

platform_handler::platform_handler(void) 
{
}

auto platform_handler::create(resource_handler & rh, glm::mat4 & proj) -> void
{
	platforms.emplace_back(3.0f, glm::vec3(-((float)platform_width) / 2.0f, 0, -((float)platform_depth) / 2.0f)); 
	platforms.back().create(rh);
	platforms.emplace_back(2.0f, glm::vec3(-((float)platform_width) / 2.0f, 0, -((float)platform_depth) / 2.0f) + glm::vec3(50.0f, 0, -40.0f));
	platforms.back().create(rh);
	platforms.emplace_back(5.0f, glm::vec3(-((float)platform_width) / 2.0f, 0, -((float)platform_depth) / 2.0f) + glm::vec3(-35.0f, 0, 30.0f));
	platforms.back().create(rh);

	create_realistic_texture("res/textures/grass/grass", grass, rh);
	create_realistic_texture("res/textures/dirt/dirt", stone, rh);

	create_shaders(proj);
}

auto platform_handler::prepare(glm::vec3 & camera, glm::vec3 & light_pos, glm::vec4 & clip_plane) -> void 
{
	// implement batch rendering system
	shaders.use();

	shaders.uniform_1f(32.0f, 3);
	shaders.uniform_3f(&camera[0], 4);
	shaders.uniform_3f(&light_pos[0], 5);
	shaders.uniform_4f(&clip_plane[0], 6);

	grass.color.bind(GL_TEXTURE_2D, 0);
	grass.normal_map.bind(GL_TEXTURE_2D, 1);

	stone.color.bind(GL_TEXTURE_2D, 2);
	stone.normal_map.bind(GL_TEXTURE_2D, 3);
}

auto platform_handler::render(glm::mat4 & view_matrix) -> void
{
	for (auto plat : platforms)
	{
		shaders.use();
		shaders.uniform_mat4(&view_matrix[0][0], 1);

		auto neg_corner = plat.negative_corner();
		shaders.uniform_3f(&neg_corner[0], 2);

		render_model(plat, GL_TRIANGLES);
	}
}

auto platform_handler::create_shaders(glm::mat4 & proj) -> void
{
	shaders.create_shader(GL_VERTEX_SHADER, "shaders/platform/vsh.shader");
	shaders.create_shader(GL_GEOMETRY_SHADER, "shaders/platform/gsh.shader");
	shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/platform/fsh.shader");
	shaders.link_shaders("vertex_position", "texture_coords");
	shaders.get_uniform_locations("projection_matrix", "view_matrix", "neg_corner", 
		"dimension", "camera_position", "light_position", "clip_plane",
		"grass_texture", "grass_normals", "stone_texture", "stone_normals");
	shaders.use(); shaders.uniform_mat4(&proj[0][0], 0);

	grass.color.bind(GL_TEXTURE_2D, 0);
	grass.normal_map.bind(GL_TEXTURE_2D, 1);

	shaders.uniform_1i(0, 7);
	shaders.uniform_1i(1, 8);

	shaders.uniform_1i(2, 9);
	shaders.uniform_1i(3, 10);
}

auto platform_handler::create_realistic_texture(std::string const & begin_dir, realistic_texture & tex, resource_handler & rh) -> void
{
	auto & color = tex.color;
	auto & normal = tex.normal_map;

	auto img_color = rh.load<image>(begin_dir + "_color.png");
	color.create();
	color.bind(GL_TEXTURE_2D);
	color.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, img_color.data, img_color.w, img_color.h);
	color.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	color.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	color.enable_mipmap(GL_TEXTURE_2D);

	auto img_norm = rh.load<image>(begin_dir + "_norm.png");
	normal.create();
	normal.bind(GL_TEXTURE_2D);
	normal.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, img_norm.data, img_norm.w, img_norm.h);
	normal.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	normal.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	normal.enable_mipmap(GL_TEXTURE_2D);
}