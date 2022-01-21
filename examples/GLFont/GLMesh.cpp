#include "GLMesh.h"
#include <glad/gl.h>
#include <assert.h>
#include <memory>
#include "GLVAO.h"

GLMesh::GLMesh(const std::shared_ptr<GLVBO> &vbo, const std::shared_ptr<GLIBO> &ibo)
: m_vbo(vbo), m_ibo(ibo)
{
  m_vao = GLVAO::create();

  m_vao->bind();
  m_vbo->bind();
  m_ibo->bind();
  m_vao->unbind();
}

void GLMesh::draw(int indexCount)
{
	m_vao->bind();
	// s_vertexBuffer->bind();
	// s_vertexArrayObject->bind();
	// s_indexBuffer->bind();

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	assert(glGetError() == GL_NO_ERROR);
}
