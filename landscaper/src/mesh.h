#pragma once

#include <glm/glm.hpp>
#include <array>

#include "buffer.h"
#include "renderable.h"

template<uint32_t Width, uint32_t Depth>
class mesh : public renderable
{
private:
	static constexpr uint32_t verts_x = Width;
	static constexpr uint32_t verts_z = Depth;
public:
	mesh(float height, glm::vec3 const & neg_corner)
		: height(height), neg_corner(neg_corner)
	{
	}
	auto create(resource_handler & rh) -> void override
	{
		create_mesh();
		create_indices();
		create_vao();
	}
	auto destroy(void) -> void override
	{

	}

	auto element_buffer(void) -> std::optional<buffer *> override { return &index_buffer; };
public:
	auto indexed(void) -> bool override { return true; };
	auto count(void) -> uint32_t override { return (Width - 1) * (Depth - 1) * 6; };
	auto index_type(void) -> GLenum override { return GL_UNSIGNED_SHORT; };
	auto index_ptr(void) -> void * override { return nullptr; };
	auto negative_corner(void) -> glm::vec3 { return glm::vec3(neg_corner.x, height, neg_corner.z); };
private:
	template <typename T>
	auto index_mesh_space(T x, T z) -> uint32_t
	{
		uint32_t xui32 = static_cast<uint32_t>(x);
		uint32_t zui32 = static_cast<uint32_t>(z);
		return xui32 + Width * zui32;
	}
	auto index_world_space(float x, float z) -> uint32_t
	{
		uint32_t xui32 = static_cast<uint32_t>(x - neg_corner.x);
		uint32_t zui32 = static_cast<uint32_t>(z - neg_corner.z);
		return xui32 + Width * zui32;
	}
	auto create_mesh(void) -> void
	{
		for (uint32_t z = 0; z < Depth; ++z)
		{
			for (uint32_t x = 0; x < Width; ++x)
			{
				float x_world = static_cast<float>(x) + static_cast<float>(neg_corner.x);
				float z_world = static_cast<float>(z) + static_cast<float>(neg_corner.z);

				uint32_t index = index_mesh_space(x, z);

				vertices[index] = glm::vec3(x_world, height, z_world);
			}
		}
		vertex_buffer.create();
		vertex_buffer.fill(vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	}
	auto create_indices(void) -> void
	{
		uint32_t index = 0;
		for (uint32_t x = 0; x < Width - 1; ++x)
		{
			for (uint32_t z = 0; z < Depth - 1; z++)
			{
				indices[index++] = index_mesh_space(x, z);
				indices[index++] = index_mesh_space(x + 1, z);
				indices[index++] = index_mesh_space(x + 1, z + 1);

				indices[index++] = index_mesh_space(x, z);
				indices[index++] = index_mesh_space(x + 1, z + 1);
				indices[index++] = index_mesh_space(x, z + 1);
			}
		}
		index_buffer.create();
		index_buffer.fill(indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	}
	auto create_vao(void) -> void
	{
		attribute vertex_attribute{ GL_FLOAT, 3, GL_FALSE, 3 * sizeof(float), nullptr, false };
		layout.create();
		layout.bind();
		layout.attach(vertex_buffer, vertex_attribute);

		unbind_vertex_layouts();
		unbind_buffers(GL_ARRAY_BUFFER);
		unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	}
private:
	glm::vec3 neg_corner;
	std::array<glm::vec3, Width * Depth> vertices;
	std::array<uint16_t, (Width - 1) * (Depth - 1) * 6> indices;

	buffer index_buffer;
	buffer vertex_buffer;

	float height;
};