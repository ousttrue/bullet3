#pragma once
#include <stdint.h>
#include <memory>

class GLTexture
{
	uint32_t m_texture;

	GLTexture(uint32_t texture) : m_texture(texture) {}

public:
	~GLTexture();
	static std::shared_ptr<GLTexture> load(const void *image, int width, int height);
	void bind();
	void unbind();
};
