#pragma once

struct PrimInternalData
{
	unsigned int m_shaderProg;
	int m_viewmatUniform;
	int m_projMatUniform;
	int m_positionUniform;
	int m_colourAttribute;
	int m_positionAttribute;
	int m_textureAttribute;
	unsigned int m_vertexBuffer;
	unsigned int m_vertexBuffer2;

	unsigned int m_vertexArrayObject;
	unsigned int m_vertexArrayObject2;

	unsigned int m_indexBuffer;
	unsigned int m_indexBuffer2;
	unsigned int m_texturehandle;
};
