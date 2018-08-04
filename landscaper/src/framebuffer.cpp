#include "framebuffer.h"
#include "texture.h"
#include "renderbuffer.h"

auto unbind_all_framebuffers(int32_t w, int32_t h) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
}

auto framebuffer::create(int32_t width, int32_t height) -> uint32_t
{
	glGenFramebuffers(1, &id);
	w = width;
	h = height;
	return id;
}

auto framebuffer::destroy(void) -> void { /* ... */ }

auto framebuffer::bind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, w, h);
}

auto framebuffer::unbind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto framebuffer::attach(texture & tex, uint32_t component, uint32_t level) -> void
{
	glFramebufferTexture(GL_FRAMEBUFFER, component, tex.id, level);
}

auto framebuffer::select_color_buffer(GLenum point) -> void
{
	glDrawBuffer(point);
}

auto framebuffer::framebuffer_status(void) -> bool
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

auto framebuffer::attach(renderbuffer & rnd, uint32_t component) -> void
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, component, GL_RENDERBUFFER, rnd.id);
}