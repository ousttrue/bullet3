#include "GLTexture.h"
#include <stdint.h>
#include "OpenGLInclude.h"
#include <assert.h>

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_texture);
}

std::shared_ptr<GLTexture> GLTexture::load(const void *image, int width, int height)
{
	uint32_t texture;
	glGenTextures(1, &texture);
	assert(texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	assert(glGetError() == GL_NO_ERROR);

	return std::shared_ptr<GLTexture>(new GLTexture(texture));
}

void GLTexture::bind()
{
	assert(glGetError() == GL_NO_ERROR);
	glActiveTexture(GL_TEXTURE0);
	assert(glGetError() == GL_NO_ERROR);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	assert(glGetError() == GL_NO_ERROR);
}

void GLTexture::setFiltering(bool use)
{
	if (use)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}
