#pragma once

#include <array>
#include "types.h"
#include "math/barry_centric.h"

template<u32 Width, u32 Depth> class platform
{
public:
	auto create(f32 height, glm::vec2 neg_corner) -> void
	{
		this->height = height;
		neg_corner_xz = neg_corner;

		create_heights();
		create_buffer();
	}

	auto operator[](u32 i) -> f32 *
	{
		return &heights[i];
	}
	auto heights_buffer(void) -> buffer &
	{
		return height_buffer;
	}

	auto get_platform_space_coord(glm::vec3 const & world_pos) -> glm::vec2 
	{
		return glm::vec2(world_pos.x - neg_corner_xz.x, world_pos.z - neg_corner_xz.y);
	}

	auto height_at(f32 x, f32 z) -> f32
	{
		f32 height;

		if (outside_platform(x, z)) return 0;

		/* difference between the tile corner and position */
		glm::vec2 position_on_tile{ x - floor(x), z - floor(z) };

		/* indices of the triangle */
		u32 indices[3];

		/* get corner of tile in platform space (starting from 0, 0) */
		glm::vec2 tile_coord{ floor(x) + static_cast<f32>(Width) / 2.0f, 
			floor(z) + static_cast<f32>(Depth) / 2.0f };

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
private:
	std::array<f32, (Width * Depth) + (4 * 4)> heights;
	glm::vec2 neg_corner_xz;

	buffer height_buffer;

	f32 height;
};