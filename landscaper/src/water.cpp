#include "water.h"
#include "render_pipeline.h"

water_handler::water_handler(void)
	: water_quad(glm::vec3{ -10000.0f, 0.0f, -10000.0f },
		         glm::vec3{ -10000.0f, 0.0f, +10000.0f },
		         glm::vec3{ +10000.0f, 0.0f, -10000.0f }, 
		         glm::vec3{ +10000.0f, 0.0f, +10000.0f }),
	 reflection_plane(0, 1, 0, -0), refraction_plane(0, -1, 0, 0) 
{
}

auto water_handler::create(resource_handler & rh, glm::mat4 & projection) -> void
{
	water_quad.create(rh);
	create_shaders(projection);
	create_reflection_fbo(reflection_width, reflection_height);
	create_refraction_fbo(refraction_width, refraction_height);
	get_dudv_map(rh);
	get_normal_map(rh);
}

auto water_handler::prepare(glm::mat4 & view, glm::vec3 & cam_pos, glm::vec3 & light, float elapsed) -> void
{
	static float move_factor = 0;
	move_factor += elapsed * 0.1f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	water_shaders.use();
	water_shaders.uniform_mat4(&view[0][0], 1);
	water_shaders.uniform_3f(&cam_pos[0], 2);
	water_shaders.uniform_1f(move_factor, 3);
	water_shaders.uniform_3f(&light[0], 4);

	refl_color.bind(GL_TEXTURE_2D, 0);
	refr_color.bind(GL_TEXTURE_2D, 1);
	refr_depth.bind(GL_TEXTURE_2D, 4);

	dudv_texture.bind(GL_TEXTURE_2D, 2);
	normal_map_texture.bind(GL_TEXTURE_2D, 3);
}
auto water_handler::render(void) -> void
{
	render_model(water_quad, GL_TRIANGLE_STRIP);
}

auto water_handler::bind_refl(void) -> void
{
	water_reflection_fbo.bind();
}

auto water_handler::bind_refr(void) -> void
{
	water_refraction_fbo.bind();
}

// getters
auto water_handler::refl_texture(void)->texture &
{
	return refl_color;
}
auto water_handler::refr_texture(void)->texture &
{
	return refr_color;
}
auto water_handler::refr_depth_texture(void)->texture &
{
	return refr_depth;
}

auto water_handler::create_reflection_fbo(int32_t w, int32_t h) -> void
{
	water_reflection_fbo.create(w, h);
	water_reflection_fbo.bind();

	create_color_texture_attachment(refl_color, water_reflection_fbo, w, h);
	create_depth_buffer_attachment(refl_depth, water_reflection_fbo, w, h);
}

auto water_handler::create_refraction_fbo(int32_t w, int32_t h) -> void
{
	water_refraction_fbo.create(w, h);
	water_refraction_fbo.bind();

	create_color_texture_attachment(refr_color, water_refraction_fbo, w, h);
	create_depth_texture_attachment(refr_depth, water_refraction_fbo, w, h);
}

auto water_handler::create_color_texture_attachment(texture & t, framebuffer & f, int32_t w, int32_t h) -> void
{
	create_color_texture(t, w, h, nullptr);
	f.attach(t, GL_COLOR_ATTACHMENT0, 0);
}

auto water_handler::create_depth_buffer_attachment(renderbuffer & r, framebuffer & f, int32_t w, int32_t h) -> void
{
	create_depth_buffer(r, w, h);
	f.attach(r, GL_DEPTH_ATTACHMENT);
}

auto water_handler:: create_depth_texture_attachment(texture & t, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	create_depth_texture(t, w, h);
	fbo.attach(t, GL_DEPTH_ATTACHMENT, 0);
}

auto water_handler::create_shaders(glm::mat4 & projection) -> void
{
	water_shaders.create_shader(GL_VERTEX_SHADER, "shaders/water/vsh.shader");
	water_shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/water/fsh.shader");
	water_shaders.link_shaders("vertex_position");
	water_shaders.get_uniform_locations("projection_matrix", "view_matrix", "camera_position", 
		"move_factor", "light_position", "reflection_texture", "refraction_texture", "dudv_map", "normal_map", "depth_texture");
	water_shaders.use();
	water_shaders.uniform_mat4(&projection[0][0], 0);
	water_shaders.uniform_1i(0, 5);
	water_shaders.uniform_1i(1, 6);
	water_shaders.uniform_1i(2, 7);
	water_shaders.uniform_1i(3, 8);
	water_shaders.uniform_1i(4, 9);
}

auto water_handler::get_dudv_map(resource_handler & rh) -> void
{
	auto dudvmap = rh.load<image>("res/textures/water/dudv_map.png");

	dudv_texture.create();
	dudv_texture.bind(GL_TEXTURE_2D);
	dudv_texture.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, dudvmap.data, dudvmap.w, dudvmap.h);

	dudv_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	dudv_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	dudv_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	dudv_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	dudv_texture.float_param(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
}

auto water_handler::get_normal_map(resource_handler & rh) -> void
{
	auto normal_map = rh.load<image>("res/textures/water/normal_map.png");
	normal_map_texture.create();
	normal_map_texture.bind(GL_TEXTURE_2D);
	normal_map_texture.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, normal_map.data, normal_map.w, normal_map.h);
	normal_map_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	normal_map_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	normal_map_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	normal_map_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

auto water_handler::refl_plane(void)->glm::vec4 &
{
	return reflection_plane;
}

auto water_handler::refr_plane(void)->glm::vec4 &
{
	return refraction_plane;
}