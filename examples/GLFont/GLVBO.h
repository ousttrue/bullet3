#pragma once
#include <memory>
#include <stdint.h>

class GLVBO
{
	uint32_t m_vbo = 0;

	GLVBO(uint32_t buffer) : m_vbo(buffer) {}

public:
	~GLVBO();
	static std::shared_ptr<GLVBO> load_vertices_dynamic(const void* bytes, size_t size);
	void bind();
	void unbind();
};
