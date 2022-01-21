#include "GLMesh.h"
#include <glad/gl.h>
#include <assert.h>
#include <memory>
#include "GLVAO.h"
#include "GLPrimitiveRenderer.h"

GLMesh::GLMesh(const std::shared_ptr<GLVBO> &vbo, const std::shared_ptr<GLIBO> &ibo)
	: m_vbo(vbo), m_ibo(ibo)
{
	m_vao = GLVAO::create();
	{
		// setup vao
		m_vao->bind();
		m_vbo->bind();
		m_ibo->bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)sizeof(PrimVec4));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)(sizeof(PrimVec4) + sizeof(PrimVec4)));
	}
	m_vao->unbind();
}

void GLMesh::draw(int indexCount)
{
	m_vao->bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	m_vao->unbind();
}
