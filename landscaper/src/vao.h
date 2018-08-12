#pragma once

#include <GL/glew.h>
#include <stdint.h>
#include "buffer.h"

extern auto unbind_vertex_layouts(void) -> void;

struct attribute
{
	attribute(u32 type, GLenum size, 
		GLenum normalized, u32 stride, void * ptr, bool d)
		: t(type), s(size), n(normalized), st(stride), p(ptr), divisor(d)
	{
	}

	u32 t;
	GLenum s;
	GLenum n;
	u32 st;
	void * p;
	bool divisor;
};

class vertex_layout
{
public:
	vertex_layout(void) = default;

	auto create(void) -> u32;
	auto destroy(void) -> void;
	auto bind(void) -> void;
	auto in_plane_attrib(buffer & buff, u32 i, attribute att) -> void;

	template <typename ... T> auto attach(buffer & buff, T ... attribs) -> void
	{
		buff.bind(GL_ARRAY_BUFFER);

		int8_t imps[] { ( add_attrib(attribs), static_cast<int8_t>(0) ) ... };
	}

	auto counter(void) -> u32;
private:
	auto add_attrib(attribute a) -> void
	{
		glEnableVertexAttribArray(count);
		if (a.divisor) glVertexAttribDivisor(count, 1);
		glVertexAttribPointer(count++, a.s, a.t, a.n, a.st, a.p);
	};
private:
	u32 count;
	u32 id;
};
