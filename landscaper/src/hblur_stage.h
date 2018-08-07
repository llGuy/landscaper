#pragma once

#include "post_proc_stage.h"

// test speed

class hblur_stage : public postproc_stage
{
public:
	auto create(int32_t w, int32_t h) -> void override;
	auto bind(void) -> void override;
	auto render(quad_2D & quad, texture & prev) -> void override;

	inline auto width(void)  -> int32_t { return w; };
	inline auto height(void) -> int32_t { return h; };
protected:
	auto create_texture(int32_t w, int32_t h) -> void override;
	auto create_depth(int32_t w, int32_t h) -> void override;
};