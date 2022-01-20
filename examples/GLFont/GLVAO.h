#pragma once
#include <stdint.h>
#include <memory>

class GLVAO
{
	uint32_t m_vao;
	GLVAO(uint32_t vao) : m_vao(vao) {}

public:
	~GLVAO();
	static std::shared_ptr<GLVAO> create();
  void bind();
  void unbind();
};
