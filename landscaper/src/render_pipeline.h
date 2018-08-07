#pragma once

#include "glow_stage.h"
#include "post_proc_stage.h"
#include "hblur_stage.h"
#include "vblur_stage.h"
#include "default_stage.h"
#include "glow_final_stage.h"

extern auto render_model(renderable & model, GLenum type) -> void;
extern auto clear(int32_t flags, float r, float g, float b) -> void;

class render_pipeline
{
public:
	auto create(int32_t w, int32_t h, resource_handler & rh) -> void;
	auto bind_default(void) -> void;
	auto bind_glow(void) -> void;

	auto finalize_process(void) -> void;
private:
	default_stage default_target;
	glow_stage glowing_stage;
	hblur_stage horizontal_blur;
	vblur_stage vertical_blur;
	glow_final_stage final_glow_stage;

	gui_quad render_quad;
	program shaders;
};