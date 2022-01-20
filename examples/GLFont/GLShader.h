#pragma once
#include "OpenGLInclude.h"
#include <stdint.h>
#include <memory>

class GLShader
{
	uint32_t m_program = 0;

	GLShader(uint32_t program) : m_program(program) {}

public:
	~GLShader();
	static std::shared_ptr<GLShader> load(const char *szVertexProg, const char *szFragmentProg);
	int getUniformLocation(const char *name);
	int getAttributeLocation(const char *name);
	void use();
	void unuse();
	void setMatrix4x4(const char *name, const float value[16]);
};
