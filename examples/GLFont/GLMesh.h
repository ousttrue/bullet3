#pragma once
#include <memory>
#include "GLVBO.h"
#include "GLVAO.h"

class GLMesh
{
	std::shared_ptr<GLVBO> m_vbo;
	std::shared_ptr<GLIBO> m_ibo;
	std::shared_ptr<GLVAO> m_vao;

public:
	GLMesh(const std::shared_ptr<GLVBO> &vbo, const std::shared_ptr<GLIBO> &ibo);
	const std::shared_ptr<GLVBO> &vbo() const { return m_vbo; }
	void draw(int indexCount);
};
