#pragma once
#include <memory>
#include <stdint.h>

class GLVBO
{
	uint32_t m_vbo = 0;

	GLVBO(uint32_t buffer) : m_vbo(buffer) {}

public:
	~GLVBO();
	static std::shared_ptr<GLVBO> load(const void* bytes, size_t size, bool isDynamic);
	void bind();
	void unbind();
	void upload(const void* bytes, size_t size, size_t offset = 0);
};

class GLIBO
{
	uint32_t m_ibo = 0;

	GLIBO(uint32_t buffer) : m_ibo(buffer) {}

public:
	~GLIBO();
	static std::shared_ptr<GLIBO> load(const void* bytes, size_t size);
	template <typename T, size_t TN>
	static std::shared_ptr<GLIBO> load(const T (&values)[TN])
	{
		return load(values, sizeof(values));
	}
	void bind();
	void unbind();
};
