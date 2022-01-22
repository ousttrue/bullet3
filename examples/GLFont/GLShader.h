#pragma once
#include <stdint.h>
#include <vcruntime.h>
#include <memory>
#include <string>

class GLShader
{
	uint32_t m_program = 0;

	GLShader(uint32_t program) : m_program(program) {}

public:
	~GLShader();
	static std::shared_ptr<GLShader> load(const char *szVertexProg, const char *szFragmentProg);
	uint32_t program() const { return m_program; }

	int getUniformLocation(const char *name);
	int getAttributeLocation(const char *name);
	void use();
	void unuse();
	// void setMatrix4x4(const char *name, const float value[16]);
	// void setFloat2(const char *name, const float value[2]);
};

struct GLUniformVarable
{
	std::string m_name;
	mutable uint32_t m_location = -1;
	GLUniformVarable(const std::string &src) : m_name(src) {}
	uint32_t getLocation(uint32_t program) const;
	void setMat4(const float *p) const;
	void setFloat2(const float *p) const;
};

class GLVertexAttribute
{
	std::string m_name;
	mutable uint32_t m_location = -1;

public:
	GLVertexAttribute(const std::string &src) : m_name(src) {}
	uint32_t getLocation(uint32_t program) const;
	void enable(unsigned int float_count, size_t stride, size_t offset) const;
};
