#include "GLMesh.h"
#include <glad/gl.h>
#include <assert.h>
#include <memory>
#include "GLVAO.h"
#include "GLPrimitiveRenderer.h"

GLMesh::GLMesh(const std::shared_ptr<GLVBO> &vbo, const std::shared_ptr<GLIBO> &ibo, const VertexAttributeLayout *layout, int attributeCount)
	: m_vbo(vbo), m_ibo(ibo)
{
	m_vao = GLVAO::create();
	{
		// setup vao
		m_vao->bind();
		m_vbo->bind();
		m_ibo->bind();
		for (int i = 0; i < attributeCount; ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, layout[i].itemCount, GL_FLOAT, GL_FALSE, layout[i].stride, (const GLvoid *)layout[i].offset);
		}
	}
	m_vao->unbind();
}

void GLMesh::draw(int indexCount)
{
	m_vao->bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	m_vao->unbind();
}
