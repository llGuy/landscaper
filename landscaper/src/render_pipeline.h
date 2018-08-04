#pragma once

#include "post_proc_stage.h"
#include "default_stage.h"

extern auto render_model(renderable & model, GLenum type) -> void;
extern auto clear(int32_t flags, float r, float g, float b) -> void;

class render_pipeline
{
public:
	auto create(int32_t w, int32_t h, resource_handler & rh) -> void;
	auto bind_default(void) -> void;

	auto finalize_process(void) -> void;
private:
	default_stage default_target;

	gui_quad render_quad;
	program shaders;
};