#pragma once

#ifdef cpp_no_optional
#include "optional.hpp"
#else
#include <optional>
#endif

#include "vao.h"
#include "texture.h"
#include "resources.h"

class renderable
{
public:
	renderable(void) = default;
	virtual ~renderable(void) = default;

	virtual auto create(resource_handler & rh) -> void = 0;
	virtual auto destroy(void) -> void = 0;
	virtual auto indexed(void) -> bool = 0;
	virtual auto count(void) -> uint32_t = 0;
	virtual auto index_type(void) -> GLenum = 0;
	virtual auto index_ptr(void) -> void * = 0;

	virtual auto element_buffer(void) -> std::optional<buffer *> { return {}; };
	virtual auto model_texture(uint32_t i) -> std::optional<texture *> { return {}; };
	virtual auto vao(void) -> vertex_layout & { return layout; };
protected:
	vertex_layout layout;
};