#pragma once

#include <GL/glew.h>
#include <stdint.h>
#include "buffer.h"

extern auto unbind_vertex_layouts(void) -> void;

struct attribute
{
	attribute(uint32_t type, GLenum size, 
		GLenum normalized, uint32_t stride, void * ptr, bool d)
		: t(type), s(size), n(normalized), st(stride), p(ptr), divisor(d)
	{
	}

	uint32_t t;
	GLenum s;
	GLenum n;
	uint32_t st;
	void * p;
	bool divisor;
};

class vertex_layout
{
public:
	vertex_layout(void) = default;

	auto create(void) -> uint32_t;
	auto destroy(void) -> void;
	auto bind(void) -> void;

	template <typename ... T> auto attach(buffer & buff, T ... attribs) -> void
	{
		buff.bind(GL_ARRAY_BUFFER);

		int8_t imps[] { ( add_attrib(attribs), static_cast<int8_t>(0) ) ... };
	}

	auto counter(void) -> uint32_t;
private:
	auto add_attrib(attribute a)
	{
		glEnableVertexAttribArray(count);
		if (a.divisor) glVertexAttribDivisor(count, 1);
		glVertexAttribPointer(count++, a.s, a.t, a.n, a.st, a.p);
	};
private:
	uint32_t count;
	uint32_t id;
};
