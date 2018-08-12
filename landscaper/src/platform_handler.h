#pragma once

#include "platform_model.h"
#include "platform.h"
#include "quad_3D.h"
#include "program.h"
#include <vector>

class platform_handler 
{
public:
	platform_handler(void);
	// creates first platform
	auto create(resource_handler & rh, glm::mat4 & proj) -> void;

	// needs batch rendering system to work
	auto prepare(glm::vec3 & camera, glm::vec3 & light_pos, glm::vec4 & clip_plane) -> void;

	auto render(glm::mat4 & view_matrix) -> void;

	auto get_ground_height(f32 x, f32 z) -> f32;
private:
	auto create_shaders(glm::mat4 & proj) -> void;
	auto create_realistic_texture(std::string const & begin_dir, realistic_texture & tex, resource_handler & rh) -> void;
private:
	default_platform_model model;
	default_platform platform1;

	realistic_texture grass;
	realistic_texture dirt;

	program shaders;
};