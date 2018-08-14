#pragma once

#include <glm/glm.hpp>
#include <array>

#include "buffer.h"
#include "platform.h"
#include "renderable.h"

namespace {

	constexpr u32 platform_width = 128;
	constexpr u32 platform_depth = 128;

}

using default_platform = platform<platform_width, platform_depth>;

/* platform model with texture coords : 
 model includes mesh on top and sides */
template<u32 Width, u32 Depth> class platform_model : public renderable
{
private:
	struct mesh_vertex { glm::vec2 position; glm::vec2 texture_coords; };

	static constexpr u32 verts_x = Width;
	static constexpr u32 verts_z = Depth;
public:
	platform_model(glm::vec3 const & neg_corner)
		: height(3.0f), neg_corner(neg_corner)
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

	auto attach_platform_instance(default_platform & platform_in) -> void
	{
		attribute height_attribute{ GL_FLOAT, 1, GL_FALSE, sizeof(f32), nullptr, false };

		layout.bind();
		layout.in_plane_attrib(platform_in.heights_buffer(), 2, height_attribute);
	}
public:
	auto indexed(void) -> bool override { return true; };
	auto count(void) -> u32 override { return (Width - 1) * (Depth - 1) * 6 + (6 * 4); };
	auto index_type(void) -> GLenum override { return GL_UNSIGNED_INT; };
	auto index_ptr(void) -> void * override { return nullptr; };
	auto negative_corner(void) -> glm::vec3 { return glm::vec3(neg_corner.x, height, neg_corner.z); };
private:
	template <typename T>
	auto index_mesh_space(T x, T z) -> u32
	{
		u32 xui32 = static_cast<u32>(x);
		u32 zui32 = static_cast<u32>(z);
		return xui32 + Width * zui32;
	}
	auto index_world_space(f32 x, f32 z) -> u32
	{
		u32 xui32 = static_cast<u32>(x - neg_corner.x);
		u32 zui32 = static_cast<u32>(z - neg_corner.z);
		return xui32 + Width * zui32;
	}
	auto create_mesh(void) -> void
	{
		for (u32 z = 0; z < Depth; ++z)
		{
			for (u32 x = 0; x < Width; ++x)
			{
				f32 x_world = static_cast<f32>(x) + static_cast<f32>(neg_corner.x);
				f32 z_world = static_cast<f32>(z) + static_cast<f32>(neg_corner.z);

				u32 index = index_mesh_space(x, z);

				vertices[index].position = glm::vec2(x_world, z_world);
				vertices[index].texture_coords = glm::vec2((f32)x / (f32)(Width), (f32)z / (f32)(Depth));
			}
		}
		create_sides_vertices();
		vertex_buffer.create();
		vertex_buffer.fill(vertices.size() * sizeof(mesh_vertex), vertices.data(), GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
	}
	auto create_sides_vertices(void) -> void
	{
		enum side { x = 1, z = 0 };

		u32 index = Width * Depth;

		/*
		
		on each side (starting at index 0)
		0 is at the height, 1 is under the water
		2 is at the height, 3, is under the water
		
		*/

		/* create x-facing side */
		glm::vec2 left ( neg_corner.x, neg_corner.z + (f32)(Depth / 2.0f));
		glm::vec2 right ( neg_corner.x + (f32)(Width) - 1, neg_corner.z + (f32)(Depth / 2.0f) );
		/* create z-facing side */
		glm::vec2 back ( neg_corner.x + (f32)(Width / 2.0f), neg_corner.z );
		glm::vec2 front ( neg_corner.x + (f32)(Width / 2.0f), neg_corner.z + (f32)(Depth) - 1 );

		auto create_side = [this] (u32 & i, glm::vec2 & a, glm::vec2 & b, side s, f32 half) -> void
		{
			glm::vec2 offset{ 0 }; offset[s] = half;
			/* at height level */
			vertices[i].position = a + offset; 
			vertices[i].texture_coords = glm::vec2 { 0, 0 };
			++i;
			vertices[i].position = a + offset;
			vertices[i].texture_coords = glm::vec2 { 0, (3.0f) / (f32)(Width) };
			++i;

			vertices[i].position = b + offset;
			vertices[i].texture_coords = glm::vec2{ 1, 0 };
			++i;
			vertices[i].position = b + offset;
			vertices[i].texture_coords = glm::vec2{ 1, (3.0f) / (f32)(Width) };
			++i;
		};

		create_side(index, right, left, x, (f32)(Width) / 2.0f - 1);
		create_side(index, right, left, x, -(f32)(Width) / 2.0f);

		create_side(index, front, back, z, (f32)(Depth) / 2.0f - 1);
		create_side(index, front, back, z, -(f32)(Depth) / 2.0f);
	}
	auto create_sides_indices(void) -> void
	{
		u32 index = (Width - 1) * (Depth - 1) * 6;
		u32 vertex = Width * Depth;

		for (; index < (Width - 1) * (Depth - 1) * 6 + (6 * 4); vertex += 4)
		{
			indices[index++] = vertex;
			indices[index++] = vertex + 1;
			indices[index++] = vertex + 2;

			indices[index++] = vertex + 2;
			indices[index++] = vertex + 1;
			indices[index++] = vertex + 3;
		}
	}
	auto create_indices(void) -> void
	{
		u32 index = 0;
		for (u32 x = 0; x < Width - 1; ++x)
		{
			for (u32 z = 0; z < Depth - 1; z++)
			{
				indices[index++] = index_mesh_space(x, z);
				indices[index++] = index_mesh_space(x + 1, z);
				indices[index++] = index_mesh_space(x + 1, z + 1);

				indices[index++] = index_mesh_space(x, z);
				indices[index++] = index_mesh_space(x + 1, z + 1);
				indices[index++] = index_mesh_space(x, z + 1);
			}
		}
		create_sides_indices();
		index_buffer.create();
		index_buffer.fill(indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	}
	auto create_vao(void) -> void
	{
		attribute vertex_attribute { GL_FLOAT, 2, GL_FALSE, 4 * sizeof(f32), nullptr, false };
		attribute texture_attribute { GL_FLOAT, 2, GL_FALSE, 4 * sizeof(f32), (void*)(sizeof(glm::vec2)), false };
		/* heights will be stored in a separate buffer */
		attribute height_attribute { GL_FLOAT, 1, GL_FALSE, sizeof(f32), nullptr, false };

		layout.create();
		layout.bind();
		layout.attach(vertex_buffer, vertex_attribute, texture_attribute, height_attribute);

		unbind_vertex_layouts();
		unbind_buffers(GL_ARRAY_BUFFER);
		unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	}
private:
	glm::vec3 neg_corner;
	std::array<mesh_vertex, (Width * Depth) + (4 * 4)> vertices;
	std::array<u32, (Width - 1) * (Depth - 1) * 6 + (6 * 4) /* indices for the sides */> indices;

	buffer index_buffer;
	buffer vertex_buffer;

	f32 height;
};

using default_platform_model = platform_model<platform_width, platform_depth>;