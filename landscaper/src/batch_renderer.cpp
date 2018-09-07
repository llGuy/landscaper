#include "batch_renderer.h"
#include "render_pipeline.h"

batch_renderer_3D::batch_renderer_3D(u32 attrib_size, u32 max_size)
	: buffer_max_size(max_size * attrib_size), size(0)
{
	attrib_buffer.create();
}

auto batch_renderer_3D::layout_count(void) -> u32
{
	return render_model->vao().counter();
}

auto batch_renderer_3D::empty_buffer(void) -> void
{
	attrib_buffer.fill<void>(buffer_max_size, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
	size = 0;
}

auto batch_renderer_3D::bind(renderable * model) -> void
{
	render_model = model;
}

auto batch_renderer_3D::draw_de(GLenum mode) -> void
{
	auto & layout = render_model->vao();
	layout.bind();
	render_model->element_buffer().value()->bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(mode, render_model->count(), render_model->index_type(), render_model->index_ptr());
	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto batch_renderer_3D::draw_deinstanced(GLenum mode) -> void
{
	auto & layout = render_model->vao();
	layout.bind();
	render_model->element_buffer().value()->bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElementsInstanced(GL_TRIANGLES, render_model->count(), 
		render_model->index_type(), render_model->index_ptr(), size);
	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}