#include "GLPrimitiveRenderer.h"
#include "GLShader.h"
#include "GLVBO.h"
#include "GLVAO.h"
#include "GLTexture.h"
#include <assert.h>

auto VIEW_MATRIX = "viewMatrix";
auto PROJECTION_MATRIX = "projMatrix";

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

static unsigned int s_indexData[6] = {0, 1, 2, 0, 2, 3};

//
// PrimInternalData
//
PrimInternalData::PrimInternalData()
{
	m_shaderProg = GLShader::load(vertexShader3D, fragmentShader3D);

	m_viewmatUniform = m_shaderProg->getUniformLocation(VIEW_MATRIX);
	if (m_viewmatUniform < 0)
	{
		assert(0);
	}
	m_projMatUniform = m_shaderProg->getUniformLocation(PROJECTION_MATRIX);
	if (m_projMatUniform < 0)
	{
		assert(0);
	}
	m_positionUniform = m_shaderProg->getUniformLocation("p");
	if (m_positionUniform < 0)
	{
		assert(0);
	}
	m_colourAttribute = m_shaderProg->getAttributeLocation("colour");
	if (m_colourAttribute < 0)
	{
		assert(0);
	}
	m_positionAttribute = m_shaderProg->getAttributeLocation("position");
	if (m_positionAttribute < 0)
	{
		assert(0);
	}
	m_textureAttribute = m_shaderProg->getAttributeLocation("texuv");
	if (m_textureAttribute < 0)
	{
		assert(0);
	}

	{
		PrimVertex vertexData[4] = {
			PrimVertex{PrimVec4(-1, -1, 0.0, 1.0), PrimVec4(1.0, 0.0, 0.0, 1.0), PrimVec2(0, 0)},
			PrimVertex{PrimVec4(-1, 1, 0.0, 1.0), PrimVec4(0.0, 1.0, 0.0, 1.0), PrimVec2(0, 1)},
			PrimVertex{PrimVec4(1, 1, 0.0, 1.0), PrimVec4(0.0, 0.0, 1.0, 1.0), PrimVec2(1, 1)},
			PrimVertex{PrimVec4(1, -1, 0.0, 1.0), PrimVec4(1.0, 1.0, 1.0, 1.0), PrimVec2(1, 0)}};

		m_vertexArrayObject = GLVAO::create();
		m_vertexArrayObject->bind();

		m_vertexBuffer = GLVBO::load(vertexData, sizeof(vertexData), true);

		m_vertexArrayObject2 = GLVAO::create();
		m_vertexArrayObject2->bind();

		m_vertexBuffer2 = GLVBO::load(nullptr, MAX_VERTICES2 * sizeof(PrimVertex), true);

		m_indexBuffer = GLIBO::load(s_indexData, sizeof(s_indexData));

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
		m_indexBuffer2 = GLIBO::load(indexData, sizeof(indexData));

		glEnableVertexAttribArray(m_positionAttribute);
		glEnableVertexAttribArray(m_colourAttribute);
		assert(glGetError() == GL_NO_ERROR);

		glEnableVertexAttribArray(m_textureAttribute);

		glVertexAttribPointer(m_positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)0);
		glVertexAttribPointer(m_colourAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)sizeof(PrimVec4));
		glVertexAttribPointer(m_textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)(sizeof(PrimVec4) + sizeof(PrimVec4)));
		assert(glGetError() == GL_NO_ERROR);

		glActiveTexture(GL_TEXTURE0);

		GLubyte *image = new GLubyte[256 * 256 * 3];
		for (int y = 0; y < 256; ++y)
		{
			//   const int	t=y>>5;
			GLubyte *pi = image + y * 256 * 3;
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

		m_texturehandle = GLTexture::load(image, 256, 256);

		delete[] image;
	}
}

void PrimInternalData::drawTexturedRect3D(PrimVertex *vertices, int numVertices, bool useRGBA)
{
	if (numVertices == 0)
	{
		return;
	}

	assert(glGetError() == GL_NO_ERROR);
	float identity[16] = {1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1};

	m_shaderProg->use();

	glUniformMatrix4fv(m_viewmatUniform, 1, false, identity);
	glUniformMatrix4fv(m_projMatUniform, 1, false, identity);

	assert(glGetError() == GL_NO_ERROR);

	m_vertexBuffer2->bind();
	m_vertexArrayObject2->bind();

	bool useFiltering = false;
	if (useFiltering)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	/*   PrimVertex vertexData[4] = {
		   v0,v1,v2,v3
		};
    */

	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(PrimVertex), vertices);

	assert(glGetError() == GL_NO_ERROR);

	PrimVec2 p(0.f, 0.f);  //?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
	if (useRGBA)
	{
		p.p[0] = 1.f;
		p.p[1] = 1.f;
	}

	glUniform2fv(m_positionUniform, 1, (const GLfloat *)&p);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(m_positionAttribute);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(m_colourAttribute);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(m_textureAttribute);

	glVertexAttribPointer(m_positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)0);
	glVertexAttribPointer(m_colourAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)sizeof(PrimVec4));
	glVertexAttribPointer(m_textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)(sizeof(PrimVec4) + sizeof(PrimVec4)));
	assert(glGetError() == GL_NO_ERROR);

	m_indexBuffer2->bind();

	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	int indexCount = (numVertices / 4) * 6;
	assert(glGetError() == GL_NO_ERROR);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	assert(glGetError() == GL_NO_ERROR);

	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);

	//glDisableVertexAttribArray(m_data.m_textureAttribute);
	assert(glGetError() == GL_NO_ERROR);

	glUseProgram(0);

	assert(glGetError() == GL_NO_ERROR);
}

//
// GLPrimitiveRenderer
//
GLPrimitiveRenderer::GLPrimitiveRenderer()
{
}

void GLPrimitiveRenderer::drawRect(float x0, float y0, float x1, float y1, float color[4])
{
	m_data.m_texturehandle->bind();
	drawTexturedRect(x0, y0, x1, y1, color, 0, 0, 1, 1);
	assert(glGetError() == GL_NO_ERROR);
}

void GLPrimitiveRenderer::drawTexturedRect3D(const PrimVertex &v0, const PrimVertex &v1, const PrimVertex &v2, const PrimVertex &v3, float viewMat[16], float projMat[16], bool useRGBA)
{
	m_data.m_shaderProg->use();
	m_data.m_shaderProg->setMatrix4x4(VIEW_MATRIX, viewMat);
	m_data.m_shaderProg->setMatrix4x4(PROJECTION_MATRIX, projMat);

	m_data.m_vertexBuffer->bind();
	m_data.m_vertexArrayObject->bind();
	bool useFiltering = false;
	if (useFiltering)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	PrimVertex vertexData[4] = {
		v0, v1, v2, v3};

	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(PrimVertex), vertexData);

	assert(glGetError() == GL_NO_ERROR);

	PrimVec2 p(0.f, 0.f);  //?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
	if (useRGBA)
	{
		p.p[0] = 1.f;
		p.p[1] = 1.f;
	}

	glUniform2fv(m_data.m_positionUniform, 1, (const GLfloat *)&p);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(m_data.m_positionAttribute);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(m_data.m_colourAttribute);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(m_data.m_textureAttribute);

	glVertexAttribPointer(m_data.m_positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)0);
	glVertexAttribPointer(m_data.m_colourAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)sizeof(PrimVec4));
	glVertexAttribPointer(m_data.m_textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), (const GLvoid *)(sizeof(PrimVec4) + sizeof(PrimVec4)));
	assert(glGetError() == GL_NO_ERROR);

	m_data.m_indexBuffer->bind();

	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	int indexCount = 6;
	assert(glGetError() == GL_NO_ERROR);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	assert(glGetError() == GL_NO_ERROR);

	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);

	//glDisableVertexAttribArray(m_data.m_textureAttribute);
	assert(glGetError() == GL_NO_ERROR);

	glUseProgram(0);

	assert(glGetError() == GL_NO_ERROR);
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

	//	int sz = m_numVerticesText;

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
	m_data.m_texturehandle->bind();
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

	//	int sz = m_numVerticesText;

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