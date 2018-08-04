#include "render_pipeline.h"
#include "detail.h"

auto render_model(renderable & model, GLenum type) -> void
{
	auto & vao = model.vao();
	vao.bind();
	if (model.indexed())
	{
		model.element_buffer().value()->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(type, model.count(), model.index_type(), model.index_ptr());
		unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	}
	else
	{
		glDrawArrays(type, 0, model.count());
	}
	unbind_vertex_layouts();
}

auto clear(int32_t flags, float r, float g, float b) -> void
{
	glClear(flags);
	glClearColor(r, g, b, 1);
}





auto render_pipeline::create(int32_t w, int32_t h, resource_handler & rh) -> void
{
	default_target.create(w, h);

	render_quad.create(rh);
	shaders.create_shader(GL_VERTEX_SHADER, "shaders/gui_quad/vsh.shader");
	shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/gui_quad/fsh.shader");
	shaders.link_shaders("vertex_position", "texture_coords");
	shaders.get_uniform_locations("model_matrix");
}

auto render_pipeline::bind_default(void) -> void
{
	default_target.bind();
}

auto render_pipeline::finalize_process(void) -> void
{
	using detail::identity_matrix;

	unbind_all_framebuffers(default_target.width(), default_target.height());
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);

	shaders.use();
	shaders.uniform_mat4(&identity_matrix[0][0], 0);
	default_target.output().bind(GL_TEXTURE_2D, 0);
	render_model(render_quad, GL_TRIANGLE_STRIP);
}