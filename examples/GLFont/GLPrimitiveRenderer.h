#pragma once
#include <memory>
#include <GLMesh.h>

struct PrimVec2
{
	PrimVec2()
	{
	}
	PrimVec2(float x, float y)
	{
		p[0] = x;
		p[1] = y;
	}
	float p[2];
};

struct PrimVec4
{
	PrimVec4() {}
	PrimVec4(float x, float y, float z, float w)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = w;
	}
	float p[4];
};

struct PrimVertex
{
	PrimVec4 position;
	PrimVec4 colour;
	PrimVec2 uv;

	static VertexAttributeLayout layout[3];
};

struct PrimInternalData
{
	std::shared_ptr<class GLShader> m_shader;
	int m_viewmatUniform;
	int m_projMatUniform;
	int m_positionUniform;
	int m_colourAttribute;
	int m_positionAttribute;
	int m_textureAttribute;

	std::shared_ptr<class GLMesh> m_mesh;
	std::shared_ptr<class GLMesh> m_mesh2;

	std::shared_ptr<class GLTexture> m_texture;
	PrimInternalData();
	void drawTexturedRect3D(PrimVertex* vertices, int numVertices, bool useRGBA);
};

#define MAX_VERTICES2 8192
class GLPrimitiveRenderer
{
	int m_screenWidth = 1;
	int m_screenHeight = 1;

	PrimInternalData m_data;

	int m_numVerticesText = 0;
	PrimVertex m_verticesText[MAX_VERTICES2];

	int m_numVerticesRect = 0;
	PrimVertex m_verticesRect[MAX_VERTICES2];

public:
	GLPrimitiveRenderer();
	PrimInternalData* getData()
	{
		return &m_data;
	}
	void drawRect(float x0, float y0, float x1, float y1, float color[4]);
	void drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA = 0);
	void drawTexturedRect3D(const PrimVertex& v0, const PrimVertex& v1, const PrimVertex& v2, const PrimVertex& v3, float viewMat[16], float projMat[16], bool useRGBA = true);
	void setScreenSize(int width, int height);
	void drawTexturedRect2(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA = 0);
	void drawTexturedRect3D2Text(bool useRGBA = true);

private:
	void drawTexturedRect2a(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA = 0);
	void flushBatchedRects();
};
