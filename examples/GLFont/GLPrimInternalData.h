#pragma once
#include <memory>

struct PrimInternalData
{
	std::shared_ptr<class GLShader> m_shaderProg;
	int m_viewmatUniform;
	int m_projMatUniform;
	int m_positionUniform;
	int m_colourAttribute;
	int m_positionAttribute;
	int m_textureAttribute;
	std::shared_ptr<class GLVBO> m_vertexBuffer;
	std::shared_ptr<class GLVBO> m_vertexBuffer2;

	std::shared_ptr<class GLVAO> m_vertexArrayObject;
	std::shared_ptr<class GLVAO> m_vertexArrayObject2;

	unsigned int m_indexBuffer;
	unsigned int m_indexBuffer2;
	unsigned int m_texturehandle;
};
