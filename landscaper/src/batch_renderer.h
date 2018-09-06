#pragma once

#include "vao.h"
#include <vector>
#include <memory>
#include "buffer.h"
#include "renderable.h"

class batch_renderer_3D
{
private:
	renderable * render_model;
	std::vector<std::unique_ptr<buffer>> buffers;
	std::vector<u32> sizes;
	u32 buffer_max_size;
public:
	batch_renderer_3D(u32 max_size);
	auto empty_buffer(u32 index);
	auto push_matrix(f32 * data, u32 index);
	auto prepare_matrix_attribute(u32 index);
	auto bind(renderable * model);
	/* draw call types */
	auto draw_de(GLenum mode);
	auto draw_deinstanced(GLenum mode);
};