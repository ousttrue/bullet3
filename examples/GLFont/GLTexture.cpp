#include "GLTexture.h"
#include "TwFonts.h"
#include <glad/gl.h>
#include <stdint.h>
#include <assert.h>

static GLuint BindFont(const CTexFont* _Font)
{
	GLuint TexID = 0;
	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _Font->m_TexWidth, _Font->m_TexHeight, 0, GL_RED, GL_UNSIGNED_BYTE, _Font->m_TexBytes);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return TexID;
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_texture);
}

std::shared_ptr<GLTexture> GLTexture::load(const void* image, int width, int height)
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

std::shared_ptr<GLTexture> GLTexture::load_tw()
{
	if (!g_DefaultNormalFont)
	{
		TwGenerateDefaultFonts();
	}
	auto texture = BindFont(g_DefaultNormalFont);
	return std::shared_ptr<GLTexture>(new GLTexture(texture));
}

std::shared_ptr<GLTexture> GLTexture::load_tw_large()
{
	if (!g_DefaultLargeFont)
	{
		TwGenerateDefaultFonts();
	}
	auto texture = BindFont(g_DefaultLargeFont);
	return std::shared_ptr<GLTexture>(new GLTexture(texture));
}

void GLTexture::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
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
