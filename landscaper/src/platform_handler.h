#pragma once

#include "quad_3D.h"
#include "mesh.h"
#include "program.h"
#include <vector>

using default_mesh = mesh<32, 32>;
using platform = default_mesh;

class platform_handler
{
public:
	platform_handler(void);
	// creates first platform
	auto create(resource_handler & rh, glm::mat4 & proj) -> void;

	// needs batch rendering system to work
	auto prepare(glm::vec3 & camera, glm::vec3 & light_pos, glm::vec4 & clip_plane) -> void;

	auto render(glm::mat4 & view_matrix) -> void;
private:
	auto create_shaders(glm::mat4 & proj) -> void;
	auto create_realistic_texture(std::string const & begin_dir, realistic_texture & tex, resource_handler & rh) -> void;
private:
	std::vector<platform> platforms;
	
	realistic_texture grass;
	realistic_texture dirt;
	realistic_texture stone;

//	quad_3D x_side, z_side;

	program shaders;
};