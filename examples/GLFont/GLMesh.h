#pragma once
#include <memory>
#include "GLVBO.h"
#include "GLVAO.h"

struct VertexAttributeLayout
{
	int itemCount;
	int stride;
	int offset;
};

class GLMesh
{
	std::shared_ptr<GLVBO> m_vbo;
	std::shared_ptr<GLIBO> m_ibo;
	std::shared_ptr<GLVAO> m_vao;

public:
	GLMesh(const std::shared_ptr<GLVBO> &vbo, const std::shared_ptr<GLIBO> &ibo, const VertexAttributeLayout *layout, int attributeCount);
	template <size_t N>
	static std::shared_ptr<GLMesh> create(const std::shared_ptr<GLVBO> &vbo, const std::shared_ptr<GLIBO> &ibo, const VertexAttributeLayout (&layout)[N])
	{
		return std::make_shared<GLMesh>(vbo, ibo, layout, N);
	}
	const std::shared_ptr<GLVBO> &vbo() const { return m_vbo; }
	void draw(int indexCount);
};
