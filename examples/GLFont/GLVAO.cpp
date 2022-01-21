#include "GLVAO.h"
#include "OpenGLInclude.h"
#include <stdint.h>
#include <memory>
#include <assert.h>

GLVAO::~GLVAO()
{
	glDeleteVertexArrays(1, &m_vao);
}

std::shared_ptr<GLVAO> GLVAO::create()
{
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	assert(vao);

	return std::shared_ptr<GLVAO>(new GLVAO(vao));
}

void GLVAO::bind()
{
	glBindVertexArray(m_vao);
}

void GLVAO::unbind()
{
	glBindVertexArray(0);
}
