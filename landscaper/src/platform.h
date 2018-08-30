#pragma once

#include <array>
#include "types.h"
#include "mound.h"
#include <optional>
#include "math/barry_centric.h"

template<u32 Width, u32 Depth> class platform
{
public:
	auto create(f32 height, glm::vec2 neg_corner) -> void
	{
		modified = false;

		this->height = height;
		neg_corner_xz = neg_corner;

		create_heights();
		create_buffer();
	}

	auto changed(void) -> bool & { return modified; }

	auto operator[](glm::vec2 const & i) -> f32 &
	{
		return heights[index(i.x, i.y)];
	}
	auto heights_buffer(void) -> buffer &
	{
		return height_buffer;
	}

	auto get_platform_space_coord(glm::vec3 const & world_pos) -> glm::vec2
	{
		return glm::vec2(world_pos.x - neg_corner_xz.x, world_pos.z - neg_corner_xz.y);
	}

	/* negative x and z corner */
	auto coordinate(void) -> glm::vec3
	{
		return glm::vec3(neg_corner_xz.x + (float)(Width) / 2.0f, 0, neg_corner_xz.y + (float)(Depth) / 2.0f);
	}

	auto update_gpu(void) -> void
	{
		height_buffer.fill(heights.size() * sizeof(f32), heights.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

		unbind_buffers(GL_ARRAY_BUFFER);
	}

	auto height_at(f32 x, f32 z) -> f32
	{
		f32 height;

		if (outside_platform(x, z)) return 0;

		/* difference between the tile corner and position */
		glm::vec2 position_on_tile{ x - floor(x), z - floor(z) };

		/* indices of the triangle */
		u32 indices[3];

		auto platform_coord = coordinate();

		/* get corner of tile in platform space (starting from 0, 0) */
		glm::vec2 tile_coord { floor(x) + static_cast<f32>(Width) / 2.0f - platform_coord.x,
			floor(z) + static_cast<f32>(Depth) / 2.0f - platform_coord.z };

		if (position_on_tile.y <= position_on_tile.x)
		{
			height = get_height(glm::vec2(1, 0), glm::vec2(1, 1), indices, tile_coord, position_on_tile);
		}
		else
		{
			height = get_height(glm::vec2(1, 1), glm::vec2(0, 1), indices, tile_coord, position_on_tile);
		}

		return height;
	}

	auto normal_at(f32 x, f32 z) -> std::optional<glm::vec3>
	{
		glm::vec3 normal;

		if (outside_platform(x, z)) return std::optional<glm::vec3>{};

		glm::vec2 position_on_tile{ x - floor(x), z - floor(z) };
		u32 tri_indices[3];

		auto platform_coord = coordinate();

		glm::vec2 tile_coord{ floor(x) + static_cast<f32>(Width) / 2.0f - platform_coord.x,
			floor(z) + static_cast<f32>(Depth) / 2.0f - platform_coord.z };

		if (position_on_tile.y <= position_on_tile.x)
		{
			normal = get_normal(tile_coord, glm::vec2(1, 0), glm::vec2(1, 1), tri_indices);
		}
		else
		{
			normal = get_normal(tile_coord, glm::vec2(1, 1), glm::vec2(0, 1), tri_indices);
		}

		return std::make_optional(normal);
	}

	template <typename T> auto is_on_platform_mesh_space(T px, T pz) -> bool
	{
		i32 x = static_cast<i32>(px);
		i32 z = static_cast<i32>(pz);

		return x > 0 && x < (Width - 1) &&
			z > 0 && z < (Depth - 1);
	}

	template <typename T, typename U> auto is_within_mesh_space(T px, T pz, U prad) -> bool
	{
		f32 x = static_cast<f32>(px);
		f32 z = static_cast<f32>(pz);
		f32 rad = static_cast<f32>(prad);

		return x > rad && Width - x > rad &&
			z > rad && Depth - z > rad;
	}
	auto outside_platform(f32 x, f32 z) -> bool
	{
		f32 dim_x = static_cast<f32>(Width) - 1.0f;
		f32 dim_z = static_cast<f32>(Depth) - 1.0f;

		f32 height = 0;

		/* if outisde of platform */
		if (x > dim_x + neg_corner_xz.x ||
			x < neg_corner_xz.x ||
			z > dim_z + neg_corner_xz.y ||
			z < neg_corner_xz.y)
		{
			return true;
		}
		else return false;
	}
private:
	auto create_heights(void) -> void
	{
		for (u32 i = 0; i < (Width * Depth); ++i)
			heights[i] = height;

		for (u32 i = 0; i < (4 * 4); ++i)
		{
			if (i % 2 == 1) heights[i + (Width * Depth)] = -1.0f;
			else heights[i + (Width * Depth)] = height;
		}
	}
	auto create_buffer(void) -> void
	{
		height_buffer.create();
		height_buffer.fill(heights.size() * sizeof(f32), heights.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

		unbind_buffers(GL_ARRAY_BUFFER);
	}
private:
	template <typename T> auto index(T x, T z) -> u32
	{
		return static_cast<u32>(x) + Width * static_cast<u32>(z);
	}
	/* offsets for to get the other 2 sides of the triangle */
	auto get_height(glm::vec2 const & offset1, glm::vec2 const & offset2,
		u32 * indices, glm::vec2 const & tile_coord, glm::vec2 const & pos_on_tile) -> f32
	{
		indices[0] = index(tile_coord.x, tile_coord.y);
		indices[1] = index(tile_coord.x + offset1.x, tile_coord.y + offset1.y);
		indices[2] = index(tile_coord.x + offset2.x, tile_coord.y + offset2.y);

		glm::vec3 coord1(0.0f, heights[indices[0]], 0.0f);
		glm::vec3 coord2(offset1.x, heights[indices[1]], offset1.y);
		glm::vec3 coord3(offset2.x, heights[indices[2]], offset2.y);

		return barry_centric(coord1, coord2, coord3, glm::vec2(pos_on_tile.x, pos_on_tile.y));
	}

	auto get_normal(glm::vec2 const & tile_coord, glm::vec2 const & offset1,
		glm::vec2 const & offset2, u32 * indices) -> glm::vec3
	{
		indices[0] = index(tile_coord.x, tile_coord.y);
		indices[1] = index(tile_coord.x + offset1.x, tile_coord.y + offset1.y);
		indices[2] = index(tile_coord.x + offset2.x, tile_coord.y + offset2.y);

		glm::vec3 a = glm::vec3(tile_coord.x, heights[indices[0]], tile_coord.y);
		glm::vec3 b = glm::vec3(tile_coord.x + offset1.x, heights[indices[1]], tile_coord.y + offset1.y);
		glm::vec3 c = glm::vec3(tile_coord.x + offset2.x, heights[indices[2]], tile_coord.y + offset2.y);

		return glm::normalize(glm::cross(c - a, b - a));
	}
private:
	std::array<f32, (Width * Depth) + (4 * 4)> heights;
	glm::vec2 neg_corner_xz;

	buffer height_buffer;

	f32 height;

	bool modified;
};