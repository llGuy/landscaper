#pragma once

#include "post_proc_stage.h"

// test speed

class vblur_stage : public postproc_stage
{
public:
	vblur_stage(i32 scale);

	auto create(i32 w, i32 h) -> void override;
	auto bind(void) -> void override;
	auto render(quad_2D & quad, texture & prev) -> void override;

	inline auto width(void)  -> i32 { return w; };
	inline auto height(void) -> i32 { return h; };
protected:
	auto create_texture(i32 w, i32 h) -> void override;
	auto create_depth(i32 w, i32 h) -> void override;
private:
	i32 scale;
};