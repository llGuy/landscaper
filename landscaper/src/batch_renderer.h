#pragma once

#include "vao.h"
#include <vector>
#include <memory>
#include "buffer.h"
#include "renderable.h"

class batch_renderer_3D
{
private:
	u32 size;
	u32 buffer_max_size;
	buffer attrib_buffer;
	renderable * render_model;
public:
	batch_renderer_3D(void) = default;
	batch_renderer_3D(u32 attrib_size, u32 max_size);
	auto layout_count(void) -> u32;
	auto empty_buffer(void) -> void;
	auto bind(renderable * model) -> void;
	/* draw call types */
	auto draw_de(GLenum mode) -> void;
	auto draw_deinstanced(GLenum mode) -> void;

	template <typename T> auto push_attrib(T && attrib) -> void;
	template <typename ... T> auto prepare_instance_attrib(T ... attribs) -> void;
};

template <typename T> auto batch_renderer_3D::push_attrib(T && attrib) -> void
{
	attrib_buffer.partial_fill(size * sizeof(T), sizeof(T), &attrib, GL_ARRAY_BUFFER);
	size++;
}

template <typename ... T> auto batch_renderer_3D::prepare_instance_attrib(T ... attribs) -> void
{
	auto & model_layout = render_model->vao();
	model_layout.bind();
	attrib_buffer.bind(GL_ARRAY_BUFFER);
	(model_layout.add_attrib(attribs), ...);
}