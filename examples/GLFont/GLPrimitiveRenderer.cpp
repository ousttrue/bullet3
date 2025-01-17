#include "GLPrimitiveRenderer.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLMesh.h"
#include <assert.h>
#include <stdlib.h>
#include <memory>

// glEnableVertexAttribArray(0);
// glEnableVertexAttribArray(1);
// glEnableVertexAttribArray(2);
// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)0);
// glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)sizeof(PrimVec4));
// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)(sizeof(PrimVec4) + sizeof(PrimVec4)));
VertexAttributeLayout PrimVertex::layout[3] = {
	VertexAttributeLayout{4, sizeof(PrimVertex), 0},
	VertexAttributeLayout{4, sizeof(PrimVertex), offsetof(PrimVertex, colour)},
	VertexAttributeLayout{2, sizeof(PrimVertex), offsetof(PrimVertex, uv)},
};

static const char *vertexShader3D =
	"#version 150   \n"
	"\n"
	"uniform mat4 viewMatrix, projMatrix;\n"
	"in vec4 position;\n"
	"in vec4 colour;\n"
	"out vec4 colourV;\n"
	"\n"
	"in vec2 texuv;\n"
	"out vec2 texuvV;\n"
	"\n"
	"\n"
	"void main (void)\n"
	"{\n"
	"    colourV = colour;\n"
	"   gl_Position = projMatrix * viewMatrix * position ;\n"
	"	texuvV=texuv;\n"
	"}\n";

static const char *fragmentShader3D =
	"#version 150\n"
	"\n"
	"uniform vec2 p;\n"
	"in vec4 colourV;\n"
	"out vec4 fragColour;\n"
	"in vec2 texuvV;\n"
	"\n"
	"uniform sampler2D Diffuse;\n"
	"\n"
	"void main(void)\n"
	"{\n"
	"	vec4 texcolor = texture(Diffuse,texuvV);\n"
	"  if (p.x==0.f)\n"
	"  {\n"
	"		texcolor = vec4(1,1,1,texcolor.x);\n"
	"  }\n"
	"   fragColour = colourV*texcolor;\n"
	"}\n";

//
// PrimInternalData
//
PrimInternalData::PrimInternalData()
{
	m_shader = GLShader::load(vertexShader3D, fragmentShader3D);
	m_viewmatUniform.getLocation(m_shader->program());
	m_projMatUniform.getLocation(m_shader->program());
	m_positionUniform.getLocation(m_shader->program());
	m_positionAttribute.getLocation(m_shader->program());
	m_colourAttribute.getLocation(m_shader->program());
	m_textureAttribute.getLocation(m_shader->program());

	{
		PrimVertex vertexData[4] = {
			PrimVertex{PrimVec4(-1, -1, 0.0, 1.0), PrimVec4(1.0, 0.0, 0.0, 1.0), PrimVec2(0, 0)},
			PrimVertex{PrimVec4(-1, 1, 0.0, 1.0), PrimVec4(0.0, 1.0, 0.0, 1.0), PrimVec2(0, 1)},
			PrimVertex{PrimVec4(1, 1, 0.0, 1.0), PrimVec4(0.0, 0.0, 1.0, 1.0), PrimVec2(1, 1)},
			PrimVertex{PrimVec4(1, -1, 0.0, 1.0), PrimVec4(1.0, 1.0, 1.0, 1.0), PrimVec2(1, 0)}};
		static unsigned int s_indexData[6] = {0, 1, 2, 0, 2, 3};
		auto vbo = GLVBO::load(vertexData, sizeof(vertexData), true);
		auto ibo = GLIBO::load(s_indexData, sizeof(s_indexData));
		m_mesh = GLMesh::create(vbo, ibo, PrimVertex::layout);
	}

	{
		unsigned int indexData[MAX_VERTICES2 * 2];
		int count = 0;
		for (int i = 0; i < MAX_VERTICES2; i += 4)
		{
			indexData[count++] = i;
			indexData[count++] = i + 1;
			indexData[count++] = i + 2;

			indexData[count++] = i;
			indexData[count++] = i + 2;
			indexData[count++] = i + 3;
		}
		auto vbo = GLVBO::load(nullptr, MAX_VERTICES2 * sizeof(PrimVertex), true);
		auto ibo = GLIBO::load(indexData, sizeof(indexData));
		m_mesh2 = GLMesh::create(vbo, ibo, PrimVertex::layout);
	}

	{
		uint8_t image[256 * 256 * 3];
		for (int y = 0; y < 256; ++y)
		{
			//   const int	t=y>>5;
			auto pi = image + y * 256 * 3;
			for (int x = 0; x < 256; ++x)
			{
				if (x < y)  //x<2||y<2||x>253||y>253)
				{
					pi[0] = 255;
					pi[1] = 0;
					pi[2] = 0;
				}
				else

				{
					pi[0] = 255;
					pi[1] = 255;
					pi[2] = 255;
				}

				pi += 3;
			}
		}
		m_texture = GLTexture::load(image, 256, 256);
	}
}

void PrimInternalData::drawTexturedRect3D(PrimVertex *vertices, int numVertices, bool useRGBA)
{
	if (numVertices == 0)
	{
		return;
	}

	float identity[16] = {1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1};

	m_shader->use();
	m_viewmatUniform.setMat4(identity);
	m_projMatUniform.setMat4(identity);

	m_texture->setFiltering(false);

	m_mesh2->vbo()->upload(vertices, numVertices * sizeof(PrimVertex));

	PrimVec2 p(0.f, 0.f);  //?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
	if (useRGBA)
	{
		p.p[0] = 1.f;
		p.p[1] = 1.f;
	}

	m_positionUniform.setFloat2(p.p);

	int indexCount = (numVertices / 4) * 6;
	m_mesh2->draw(indexCount);
	m_shader->unuse();
}

//
// GLPrimitiveRenderer
//
GLPrimitiveRenderer::GLPrimitiveRenderer()
{
}

void GLPrimitiveRenderer::drawRect(float x0, float y0, float x1, float y1, float color[4])
{
	m_data.m_texture->bind();
	drawTexturedRect(x0, y0, x1, y1, color, 0, 0, 1, 1);
}

void GLPrimitiveRenderer::drawTexturedRect3D(const PrimVertex &v0, const PrimVertex &v1, const PrimVertex &v2, const PrimVertex &v3, float viewMat[16], float projMat[16], bool useRGBA)
{
	m_data.m_shader->use();
	m_data.m_viewmatUniform.setMat4(viewMat);
	m_data.m_projMatUniform.setMat4(projMat);

	m_data.m_texture->setFiltering(false);

	PrimVertex vertexData[4] = {
		v0, v1, v2, v3};
	m_data.m_mesh->vbo()->upload(vertexData, 4 * sizeof(PrimVertex));

	PrimVec2 p(0.f, 0.f);  //?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
	if (useRGBA)
	{
		p.p[0] = 1.f;
		p.p[1] = 1.f;
	}
	m_data.m_positionUniform.setFloat2(p.p);

	int indexCount = 6;
	m_data.m_mesh->draw(indexCount);
	m_data.m_shader->unuse();
}

void GLPrimitiveRenderer::drawTexturedRect3D2Text(bool useRGBA)
{
	m_data.drawTexturedRect3D(&m_verticesText[0], m_numVerticesText, useRGBA);
	m_numVerticesText = 0;
}

void GLPrimitiveRenderer::drawTexturedRect2a(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA)
{
	PrimVertex vertexData[4] = {
		PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(m_screenWidth), 1.f - 2.f * y0 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v0)},
		PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(m_screenWidth), 1.f - 2.f * y1 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v1)},
		PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(m_screenWidth), 1.f - 2.f * y1 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v1)},
		PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(m_screenWidth), 1.f - 2.f * y0 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v0)}};

	m_verticesRect[m_numVerticesRect++] = vertexData[0];
	m_verticesRect[m_numVerticesRect++] = vertexData[1];
	m_verticesRect[m_numVerticesRect++] = vertexData[2];
	m_verticesRect[m_numVerticesRect++] = vertexData[3];

	if (m_numVerticesRect >= MAX_VERTICES2)
	{
		flushBatchedRects();
	}
}

void GLPrimitiveRenderer::flushBatchedRects()
{
	m_data.m_texture->bind();
	m_data.drawTexturedRect3D(m_verticesRect, m_numVerticesRect, 0);
	m_numVerticesRect = 0;
}
void GLPrimitiveRenderer::drawTexturedRect2(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA)
{
	PrimVertex vertexData[4] = {
		PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(m_screenWidth), 1.f - 2.f * y0 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v0)},
		PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(m_screenWidth), 1.f - 2.f * y1 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v1)},
		PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(m_screenWidth), 1.f - 2.f * y1 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v1)},
		PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(m_screenWidth), 1.f - 2.f * y0 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v0)}};

	m_verticesText[m_numVerticesText++] = vertexData[0];
	m_verticesText[m_numVerticesText++] = vertexData[1];
	m_verticesText[m_numVerticesText++] = vertexData[2];
	m_verticesText[m_numVerticesText++] = vertexData[3];

	if (m_numVerticesText >= MAX_VERTICES2)
	{
		m_data.drawTexturedRect3D(m_verticesText, m_numVerticesText, useRGBA);
		m_numVerticesText = 0;
	}
}

void GLPrimitiveRenderer::drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA)
{
	float identity[16] = {1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1};
	PrimVertex vertexData[4] = {
		PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(m_screenWidth), 1.f - 2.f * y0 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v0)},
		PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(m_screenWidth), 1.f - 2.f * y1 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v1)},
		PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(m_screenWidth), 1.f - 2.f * y1 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v1)},
		PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(m_screenWidth), 1.f - 2.f * y0 / float(m_screenHeight), 0.f, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v0)}};

	drawTexturedRect3D(vertexData[0], vertexData[1], vertexData[2], vertexData[3], identity, identity, useRGBA);
}

void GLPrimitiveRenderer::setScreenSize(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}
