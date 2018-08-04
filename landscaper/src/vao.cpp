#include "vao.h"

auto unbind_vertex_layouts(void) -> void
{
	glBindVertexArray(0);
}

auto vertex_layout::create(void) -> uint32_t
{
	count = 0;

	glGenVertexArrays(1, &id);
	return id;
}

auto vertex_layout::destroy(void) -> void
{
	glDeleteVertexArrays(1, &id);
}

auto vertex_layout::bind(void) -> void
{
	glBindVertexArray(id);
}

auto vertex_layout::counter(void) -> uint32_t
{
	return count;
}