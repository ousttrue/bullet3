#include "GLVBO.h"
#include <stdint.h>
#include <assert.h>
#include <memory>
#include "OpenGLInclude.h"

GLVBO::~GLVBO()
{
	glDeleteBuffers(1, &m_vbo);
}

std::shared_ptr<GLVBO> GLVBO::load(const void* bytes, size_t size, bool isDynamic)
{
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	assert(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, bytes, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	GLuint err = glGetError();
	assert(err == GL_NO_ERROR);
	return std::shared_ptr<GLVBO>(new GLVBO(vbo));
}

void GLVBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void GLVBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVBO::upload(const void* bytes, size_t size)
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, bytes);
	assert(glGetError() == GL_NO_ERROR);
	unbind();
}

//
//
//
GLIBO::~GLIBO()
{
	glDeleteBuffers(1, &m_ibo);
}

std::shared_ptr<GLIBO> GLIBO::load(const void* bytes, size_t size)
{
	uint32_t ibo;
	glGenBuffers(1, &ibo);
	assert(ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, bytes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return std::shared_ptr<GLIBO>(new GLIBO(ibo));
}

void GLIBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

void GLIBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
