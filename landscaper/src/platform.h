#pragma once

#include <array>
#include "types.h"

template<u32 Width, u32 Depth> class platform
{
public:
	auto create(f32 height) -> void
	{
		this->height = height;

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
	std::array<f32, (Width * Depth) + (4 * 4)> heights;

	buffer height_buffer;

	f32 height;
};