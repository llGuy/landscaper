#include "batch_renderer.h"

batch_renderer_3D::batch_renderer_3D(u32 max_size)
	: buffer_max_size(max_size)
{
}

auto batch_renderer_3D::empty_buffer(u32 index)
{

}

auto batch_renderer_3D::push_matrix(f32 * data, u32 index)
{

}

auto batch_renderer_3D::prepare_matrix_attribute(u32 index)
{

}

auto batch_renderer_3D::bind(renderable * model)
{
	render_model = model;
}

auto batch_renderer_3D::draw_de(GLenum mode)
{

}

auto batch_renderer_3D::draw_deinstanced(GLenum mode)
{

}