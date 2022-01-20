#include "GLVBO.h"
#include <stdint.h>
#include <assert.h>
#include <memory>
#include "OpenGLInclude.h"

GLVBO::~GLVBO()
{
	glDeleteBuffers(1, &m_vbo);
}

std::shared_ptr<GLVBO> GLVBO::load_vertices_dynamic(const void* bytes, size_t size)
{
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	assert(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, bytes, GL_DYNAMIC_DRAW);
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
