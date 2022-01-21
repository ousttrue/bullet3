#include "GlfwApp.h"
#include <CommonExampleInterface.h>
#include <fontstash.h>
#include "GLRenderToTexture.h"
#include "opengl_fontstashcallbacks.h"
#include "ShapeData.h"
#include <GLTexture.h>
#include <Bullet3Common/b3Quaternion.h>
#include <stb_image_write.h>
#include <memory>
#include <stdexcept>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <GLFW/glfw3.h>
#include <OpenSans.h>
#include <TwFonts.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <assert.h>
#include <glad/gl.h>

static void window_close_callback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void SimpleResizeCallback(GLFWwindow* window, int width, int height)
{
	auto gApp = (GlfwApp*)glfwGetWindowUserPointer(window);
	for (auto& callback : gApp->resizeCallback)
	{
		callback(width, height);
	}
}

static void SimpleKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto gApp = (GlfwApp*)glfwGetWindowUserPointer(window);
	for (auto& callback : gApp->keyboardCallback)
	{
		if (callback(key, action))
		{
			break;
		}
	}
}

int s_x = 0;
int s_y = 0;
static void SimpleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto gApp = (GlfwApp*)glfwGetWindowUserPointer(window);
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			button = 2;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			button = 1;
			break;
	}

	ButtonFlags flags = {};
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
	{
		flags = (ButtonFlags)(flags | ButtonFlagsAlt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		flags = (ButtonFlags)(flags | ButtonFlagsCtrl);
	}

	for (auto& callback : gApp->mouseButtonCallback)
	{
		if (callback(button, action, s_x, s_y, flags))
		{
			break;
		}
	}
}

static void SimpleMouseMoveCallback(GLFWwindow* window, double x, double y)
{
	auto gApp = (GlfwApp*)glfwGetWindowUserPointer(window);
	s_x = x;
	s_y = y;

	for (auto& callback : gApp->mouseMoveCallback)
	{
		if (callback(x, y))
		{
			break;
		}
	}
}

static void SimpleWheelCallback(GLFWwindow* window, double deltax, double deltay)
{
	auto gApp = (GlfwApp*)glfwGetWindowUserPointer(window);
	for (auto& callback : gApp->wheelCallback)
	{
		if (callback(deltax, deltay))
		{
			break;
		}
	}
}

struct MyRenderCallbacks : public RenderCallbacks
{
	GLInstancingRenderer* m_instancingRenderer;

	b3AlignedObjectArray<unsigned char> m_rgbaTexture;
	float m_color[4];
	float m_worldPosition[3];
	float m_worldOrientation[4];

	int m_textureIndex;

	MyRenderCallbacks(GLInstancingRenderer* instancingRenderer)
		: m_instancingRenderer(instancingRenderer),
		  m_textureIndex(-1)
	{
		for (int i = 0; i < 4; i++)
		{
			m_color[i] = 1;
			m_worldOrientation[i] = 0;
		}
		m_worldPosition[0] = 0;
		m_worldPosition[1] = 0;
		m_worldPosition[2] = 0;

		m_worldOrientation[0] = 0;
		m_worldOrientation[1] = 0;
		m_worldOrientation[2] = 0;
		m_worldOrientation[3] = 1;
	}
	virtual ~MyRenderCallbacks()
	{
		m_rgbaTexture.clear();
	}

	virtual void setWorldPosition(float pos[3])
	{
		for (int i = 0; i < 3; i++)
		{
			m_worldPosition[i] = pos[i];
		}
	}

	virtual void setWorldOrientation(float orn[4])
	{
		for (int i = 0; i < 4; i++)
		{
			m_worldOrientation[i] = orn[i];
		}
	}

	virtual void setColorRGBA(float color[4])
	{
		for (int i = 0; i < 4; i++)
		{
			m_color[i] = color[i];
		}
	}
	virtual void updateTexture(sth_texture* texture, sth_glyph* glyph, int textureWidth, int textureHeight)
	{
		if (glyph)
		{
			m_rgbaTexture.resize(textureWidth * textureHeight * 3);
			for (int i = 0; i < textureWidth * textureHeight; i++)
			{
				m_rgbaTexture[i * 3 + 0] = texture->m_texels[i];
				m_rgbaTexture[i * 3 + 1] = texture->m_texels[i];
				m_rgbaTexture[i * 3 + 2] = texture->m_texels[i];
			}
			bool flipPixelsY = false;
			m_instancingRenderer->updateTexture(m_textureIndex, &m_rgbaTexture[0], flipPixelsY);
		}
		else
		{
			if (textureWidth && textureHeight)
			{
				texture->m_texels = (unsigned char*)malloc(textureWidth * textureHeight);
				memset(texture->m_texels, 0, textureWidth * textureHeight);
				if (m_textureIndex < 0)
				{
					m_rgbaTexture.resize(textureWidth * textureHeight * 3);
					bool flipPixelsY = false;
					m_textureIndex = m_instancingRenderer->registerTexture(&m_rgbaTexture[0], textureWidth, textureHeight, flipPixelsY);

					int strideInBytes = 9 * sizeof(float);
					int numVertices = sizeof(cube_vertices_textured) / strideInBytes;
					int numIndices = sizeof(cube_indices) / sizeof(int);

					float halfExtentsX = 1;
					float halfExtentsY = 1;
					float halfExtentsZ = 1;
					float textureScaling = 4;

					b3AlignedObjectArray<GfxVertexFormat1> verts;
					verts.resize(numVertices);
					for (int i = 0; i < numVertices; i++)
					{
						verts[i].x = halfExtentsX * cube_vertices_textured[i * 9];
						verts[i].y = halfExtentsY * cube_vertices_textured[i * 9 + 1];
						verts[i].z = halfExtentsZ * cube_vertices_textured[i * 9 + 2];
						verts[i].w = cube_vertices_textured[i * 9 + 3];
						verts[i].nx = cube_vertices_textured[i * 9 + 4];
						verts[i].ny = cube_vertices_textured[i * 9 + 5];
						verts[i].nz = cube_vertices_textured[i * 9 + 6];
						verts[i].u = cube_vertices_textured[i * 9 + 7] * textureScaling;
						verts[i].v = cube_vertices_textured[i * 9 + 8] * textureScaling;
					}

					int shapeId = m_instancingRenderer->registerShape(&verts[0].x, numVertices, cube_indices, numIndices, B3_GL_TRIANGLES, m_textureIndex);
					b3Vector3 pos = b3MakeVector3(0, 0, 0);
					b3Quaternion orn(0, 0, 0, 1);
					b3Vector4 color = b3MakeVector4(1, 1, 1, 1);
					b3Vector3 scaling = b3MakeVector3(.1, .1, .1);
					//m_instancingRenderer->registerGraphicsInstance(shapeId, pos, orn, color, scaling);
					m_instancingRenderer->writeTransforms();
				}
				else
				{
					b3Assert(0);
				}
			}
			else
			{
				delete texture->m_texels;
				texture->m_texels = 0;
				//there is no m_instancingRenderer->freeTexture (yet), all textures are released at reset/deletion of the renderer
			}
		}
	}
	virtual void render(sth_texture* texture)
	{
		int index = 0;

		float width = 1;
		b3AlignedObjectArray<unsigned int> indices;
		indices.resize(texture->nverts);
		for (int i = 0; i < indices.size(); i++)
		{
			indices[i] = i;
		}

		m_instancingRenderer->drawTexturedTriangleMesh(m_worldPosition, m_worldOrientation, &texture->newverts[0].position.p[0], texture->nverts, &indices[0], indices.size(), m_color, m_textureIndex);
	}
};

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void printGLString(const char* name, GLenum s)
{
	const char* v = (const char*)glGetString(s);
	printf("%s = %s\n", name, v);
}

///
/// GlfwApp
///
GlfwApp::GlfwApp()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		throw std::runtime_error("glfwInit");
	}
}

GlfwApp::~GlfwApp()
{
	delete m_instancingRenderer;
	delete m_primRenderer;
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool GlfwApp::createWindow(const b3gWindowConstructionInfo& ci)
{
	m_window = glfwCreateWindow(ci.m_width, ci.m_height, ci.m_title, NULL, NULL);
	if (!m_window)
	{
		return false;
	}

	glfwSetWindowCloseCallback(m_window, window_close_callback);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetWindowSizeCallback(m_window, SimpleResizeCallback);
	glfwSetCursorPosCallback(m_window, SimpleMouseMoveCallback);
	glfwSetMouseButtonCallback(m_window, SimpleMouseButtonCallback);
	glfwSetKeyCallback(m_window, SimpleKeyboardCallback);
	glfwSetScrollCallback(m_window, SimpleWheelCallback);
	glfwMakeContextCurrent(m_window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	setWindowTitle(ci.m_title);

	b3Assert(glGetError() == GL_NO_ERROR);

	{
		printGLString("Version", GL_VERSION);
		printGLString("Vendor", GL_VENDOR);
		printGLString("Renderer", GL_RENDERER);
	}

	glClearColor(m_backgroundColorRGB[0],
				 m_backgroundColorRGB[1],
				 m_backgroundColorRGB[2],
				 1.f);

	startRendering();
	auto width = getWidth();
	auto height = getHeight();

	b3Assert(glGetError() == GL_NO_ERROR);

	resizeCallback.push_back(
		[gApp = this](int width, int height)
		{
			glViewport(0, 0, width, height);
			if (gApp->m_instancingRenderer)
				gApp->m_instancingRenderer->resize(width, height);
			if (gApp->m_primRenderer)
				gApp->m_primRenderer->setScreenSize(width, height);
		});
	keyboardCallback.push_back(
		[self = this](int keycode, int state)
		{
			if (keycode == B3G_ESCAPE)
			{
				self->setRequestExit();
				return true;
			}
			return false;
		});

	glClearColor(m_backgroundColorRGB[0],
				 m_backgroundColorRGB[1],
				 m_backgroundColorRGB[2],
				 1.f);

	b3Assert(glGetError() == GL_NO_ERROR);

	m_parameterInterface = 0;

	int maxNumObjectCapacity = 128 * 1024;
	int maxShapeCapacityInBytes = 128 * 1024 * 1024;
	m_instancingRenderer = new GLInstancingRenderer(maxNumObjectCapacity, maxShapeCapacityInBytes);

	m_primRenderer = new GLPrimitiveRenderer();
	m_primRenderer->setScreenSize(width, height);
	m_renderer = m_instancingRenderer;

	m_instancingRenderer->init();
	m_instancingRenderer->resize(width, height);
	m_primRenderer->setScreenSize(width, height);
	b3Assert(glGetError() == GL_NO_ERROR);

	m_instancingRenderer->InitShaders();

	m_largeFontTextureId = GLTexture::load_tw_large();

	{
		m_renderCallbacks = new OpenGL2RenderCallbacks(m_primRenderer->getData());
		m_renderCallbacks2 = new MyRenderCallbacks(m_instancingRenderer);
		m_fontStash2 = std::make_shared<FontStash>(512, 512, m_renderCallbacks2);
		m_fontStash = std::make_shared<FontStash>(512, 512, m_renderCallbacks);  //256,256);//,1024);//512,512);

		b3Assert(glGetError() == GL_NO_ERROR);

		if (!m_fontStash)
		{
			b3Warning("Could not create stash");
			//fprintf(stderr, "Could not create stash.\n");
		}

		if (!m_fontStash2)
		{
			b3Warning("Could not create fontStash2");
		}

		unsigned char* data2 = OpenSansData;
		unsigned char* data = (unsigned char*)data2;
		if (!(m_droidRegular = m_fontStash->add_font_from_memory(data)))
		{
			b3Warning("error!\n");
		}
		if (!(m_droidRegular2 = m_fontStash2->add_font_from_memory(data)))
		{
			b3Warning("error!\n");
		}

		b3Assert(glGetError() == GL_NO_ERROR);
	}
	return true;
}

void GlfwApp::drawText3D(const char* txt, float position[3], float orientation[4], float color[4], float size, int optionFlag)
{
	B3_PROFILE("GlfwApp::drawText3D");
	float viewMat[16];
	float projMat[16];
	CommonCameraInterface* cam = m_instancingRenderer->getActiveCamera();

	cam->getCameraViewMatrix(viewMat);
	cam->getCameraProjectionMatrix(projMat);

	float camPos[4];
	cam->getCameraPosition(camPos);
	//b3Vector3 cp= b3MakeVector3(camPos[0],camPos[2],camPos[1]);
	//b3Vector3 p = b3MakeVector3(worldPosX,worldPosY,worldPosZ);
	//float dist = (cp-p).length();
	//float dv = 0;//dist/1000.f;
	//
	//printf("str = %s\n",unicodeText);

	float dx = 0;

	//int measureOnly=0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int viewport[4] = {0, 0, cam->getScreenWidth(), cam->getScreenHeight()};

	float posX = position[0];
	float posY = position[1];
	float posZ = position[2];
	float winx, winy, winz;

	if (optionFlag & CommonGraphicsApp::eDrawText3D_OrtogonalFaceCamera)
	{
		if (!projectWorldCoordToScreen(position[0], position[1], position[2], viewMat, projMat, viewport, &winx, &winy, &winz))
		{
			return;
		}
		posX = winx;
		posY = cam->getScreenHeight() / 2 + (cam->getScreenHeight() / 2) - winy;
		posZ = 0.f;
	}

	if (optionFlag & CommonGraphicsApp::eDrawText3D_TrueType)
	{
		bool measureOnly = false;

		float fontSize = 64;  //512;//128;

		if (optionFlag & CommonGraphicsApp::eDrawText3D_OrtogonalFaceCamera)
		{
			m_fontStash->draw_text(
				m_droidRegular, fontSize, posX, posY,
				txt, &dx, cam->getScreenWidth(), cam->getScreenHeight(), measureOnly, m_retinaScale, color);
			m_fontStash->end_draw();
			m_fontStash->flush_draw();
		}
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_renderCallbacks2->setColorRGBA(color);

			m_renderCallbacks2->setWorldPosition(position);
			m_renderCallbacks2->setWorldOrientation(orientation);

			m_fontStash2->draw_text3D(
				m_droidRegular2, fontSize, 0, 0, 0,
				txt, &dx, size, color, 0);
			m_fontStash2->end_draw();
			m_fontStash2->flush_draw();
			glDisable(GL_BLEND);
		}
	}
	else
	{
		//float width = 0.f;
		int pos = 0;
		//float color[]={0.2f,0.2,0.2f,1.f};
		m_largeFontTextureId->bind();

		//float width = r.x;
		//float extraSpacing = 0.;

		float startX = posX;
		float startY = posY + g_DefaultLargeFont->m_CharHeight * size;
		float z = position[2];  //2.f*winz-1.f;//*(far

		if (optionFlag & CommonGraphicsApp::eDrawText3D_OrtogonalFaceCamera)
		{
			posX = winx;
			posY = cam->getScreenHeight() / 2 + (cam->getScreenHeight() / 2) - winy;
			z = 2.f * winz - 1.f;
			startY = posY - g_DefaultLargeFont->m_CharHeight * size;
		}

		while (txt[pos])
		{
			int c = txt[pos];
			//r.h = g_DefaultNormalFont->m_CharHeight;
			//r.w = g_DefaultNormalFont->m_CharWidth[c]+extraSpacing;
			float endX = startX + g_DefaultLargeFont->m_CharWidth[c] * size;
			if (optionFlag & CommonGraphicsApp::eDrawText3D_OrtogonalFaceCamera)
			{
				endX = startX + g_DefaultLargeFont->m_CharWidth[c] * size;
			}
			float endY = posY;

			//float currentColor[]={1.f,1.,0.2f,1.f};

			//	m_primRenderer->drawTexturedRect(startX, startY, endX, endY, currentColor,g_DefaultLargeFont->m_CharU0[c],g_DefaultLargeFont->m_CharV0[c],g_DefaultLargeFont->m_CharU1[c],g_DefaultLargeFont->m_CharV1[c]);
			float u0 = g_DefaultLargeFont->m_CharU0[c];
			float u1 = g_DefaultLargeFont->m_CharU1[c];
			float v0 = g_DefaultLargeFont->m_CharV0[c];
			float v1 = g_DefaultLargeFont->m_CharV1[c];
			//float color[4] = {currentColor[0],currentColor[1],currentColor[2],currentColor[3]};
			float x0 = startX;
			float x1 = endX;
			float y0 = startY;
			float y1 = endY;
			int screenWidth = cam->getScreenWidth();
			int screenHeight = cam->getScreenHeight();

			float identity[16] = {1, 0, 0, 0,
								  0, 1, 0, 0,
								  0, 0, 1, 0,
								  0, 0, 0, 1};
			if (optionFlag & CommonGraphicsApp::eDrawText3D_OrtogonalFaceCamera)
			{
				PrimVertex vertexData[4] = {
					PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(screenWidth), 1.f - 2.f * y0 / float(screenHeight), z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v0)},
					PrimVertex{PrimVec4(-1.f + 2.f * x0 / float(screenWidth), 1.f - 2.f * y1 / float(screenHeight), z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v1)},
					PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(screenWidth), 1.f - 2.f * y1 / float(screenHeight), z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v1)},
					PrimVertex{PrimVec4(-1.f + 2.f * x1 / float(screenWidth), 1.f - 2.f * y0 / float(screenHeight), z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v0)}};
				m_primRenderer->drawTexturedRect3D(vertexData[0], vertexData[1], vertexData[2], vertexData[3], identity, identity, false);
			}
			else
			{
				PrimVertex vertexData[4] = {
					PrimVertex{PrimVec4(x0, y0, z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v0)},
					PrimVertex{PrimVec4(x0, y1, z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u0, v1)},
					PrimVertex{PrimVec4(x1, y1, z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v1)},
					PrimVertex{PrimVec4(x1, y0, z, 1.f), PrimVec4(color[0], color[1], color[2], color[3]), PrimVec2(u1, v0)}};

				m_primRenderer->drawTexturedRect3D(vertexData[0], vertexData[1], vertexData[2], vertexData[3], viewMat, projMat, false);
			}
			//DrawTexturedRect(0,r,g_DefaultNormalFont->m_CharU0[c],g_DefaultNormalFont->m_CharV0[c],g_DefaultNormalFont->m_CharU1[c],g_DefaultNormalFont->m_CharV1[c]);
			//	DrawFilledRect(r);

			startX = endX;
			//startY = endY;

			pos++;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_BLEND);
}

void GlfwApp::drawText3D(const char* txt, float worldPosX, float worldPosY, float worldPosZ, float size1)
{
	float position[3] = {worldPosX, worldPosY, worldPosZ};
	float orientation[4] = {0, 0, 0, 1};
	float color[4] = {0, 0, 0, 1};
	int optionFlags = CommonGraphicsApp::eDrawText3D_OrtogonalFaceCamera;
	drawText3D(txt, position, orientation, color, size1, optionFlags);
}

// void GlfwApp::drawText(const char* txt, int posXi, int posYi, float size, float colorRGBA[4])
// {
// 	float posX = (float)posXi;
// 	float posY = (float)posYi;
// 	float dx = 0;
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	bool measureOnly = false;
// 	auto cam = m_instancingRenderer->getActiveCamera();
// 	float fontSize = 64 * size;  //512;//128;
// 	m_fontStash->draw_text(
// 		m_droidRegular, fontSize, posX, posY,
// 		txt, &dx, cam->getScreenWidth(),
// 		cam->getScreenHeight(),
// 		measureOnly,
// 		m_retinaScale, colorRGBA);
// 	m_fontStash->end_draw();
// 	m_fontStash->flush_draw();
// 	glDisable(GL_BLEND);
// }

void GlfwApp::drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_primRenderer->drawTexturedRect(x0, y0, x1, y1, color, u0, v0, u1, v1, useRGBA);
	glDisable(GL_BLEND);
}

int GlfwApp::registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex, float textureScaling)
{
	int strideInBytes = 9 * sizeof(float);
	int numVertices = sizeof(cube_vertices_textured) / strideInBytes;
	int numIndices = sizeof(cube_indices) / sizeof(int);

	b3AlignedObjectArray<GfxVertexFormat1> verts;
	verts.resize(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		verts[i].x = halfExtentsX * cube_vertices_textured[i * 9];
		verts[i].y = halfExtentsY * cube_vertices_textured[i * 9 + 1];
		verts[i].z = halfExtentsZ * cube_vertices_textured[i * 9 + 2];
		verts[i].w = cube_vertices_textured[i * 9 + 3];
		verts[i].nx = cube_vertices_textured[i * 9 + 4];
		verts[i].ny = cube_vertices_textured[i * 9 + 5];
		verts[i].nz = cube_vertices_textured[i * 9 + 6];
		verts[i].u = cube_vertices_textured[i * 9 + 7] * textureScaling;
		verts[i].v = cube_vertices_textured[i * 9 + 8] * textureScaling;
	}

	int shapeId = m_instancingRenderer->registerShape(&verts[0].x, numVertices, cube_indices, numIndices, B3_GL_TRIANGLES, textureIndex);
	return shapeId;
}

void GlfwApp::registerGrid(int cells_x, int cells_z, float color0[4], float color1[4])
{
	auto cam = m_instancingRenderer->getActiveCamera();
	b3Vector3 cubeExtents = b3MakeVector3(0.5, 0.5, 0.5);
	double halfHeight = 0.1;
	cubeExtents[cam->getCameraUpAxis()] = halfHeight;
	int cubeId = registerCubeShape(cubeExtents[0], cubeExtents[1], cubeExtents[2]);
	b3Quaternion orn(0, 0, 0, 1);
	b3Vector3 center = b3MakeVector3(0, 0, 0, 1);
	b3Vector3 scaling = b3MakeVector3(1, 1, 1, 1);

	for (int i = 0; i < cells_x; i++)
	{
		for (int j = 0; j < cells_z; j++)
		{
			float* color = 0;
			if ((i + j) % 2 == 0)
			{
				color = (float*)color0;
			}
			else
			{
				color = (float*)color1;
			}
			if (cam->getCameraUpAxis() == 1)
			{
				center = b3MakeVector3((i + 0.5f) - cells_x * 0.5f, -halfHeight, (j + 0.5f) - cells_z * 0.5f);
			}
			else
			{
				center = b3MakeVector3((i + 0.5f) - cells_x * 0.5f, (j + 0.5f) - cells_z * 0.5f, -halfHeight);
			}
			m_instancingRenderer->registerGraphicsInstance(cubeId, center, orn, color, scaling);
		}
	}
}

int GlfwApp::registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId)
{
	int red = 255;
	int green = 0;
	int blue = 128;
	if (textureId < 0)
	{
		if (m_textureId < 0)
		{
			int texWidth = 1024;
			int texHeight = 1024;
			b3AlignedObjectArray<unsigned char> texels;
			texels.resize(texWidth * texHeight * 3);
			for (int i = 0; i < texWidth * texHeight * 3; i++)
				texels[i] = 255;

			for (int i = 0; i < texWidth; i++)
			{
				for (int j = 0; j < texHeight; j++)
				{
					int a = i < texWidth / 2 ? 1 : 0;
					int b = j < texWidth / 2 ? 1 : 0;

					if (a == b)
					{
						texels[(i + j * texWidth) * 3 + 0] = red;
						texels[(i + j * texWidth) * 3 + 1] = green;
						texels[(i + j * texWidth) * 3 + 2] = blue;
						//					texels[(i+j*texWidth)*4+3] = 255;
					}
					/*else
					{
					texels[i*3+0+j*texWidth] = 255;
					texels[i*3+1+j*texWidth] = 255;
					texels[i*3+2+j*texWidth] = 255;
					}
					*/
				}
			}

			m_textureId = m_instancingRenderer->registerTexture(&texels[0], texWidth, texHeight);
		}
		textureId = m_textureId;
	}

	int strideInBytes = 9 * sizeof(float);

	int graphicsShapeIndex = -1;

	switch (lod)
	{
		case SPHERE_LOD_POINT_SPRITE:
		{
			int numVertices = sizeof(point_sphere_vertices) / strideInBytes;
			int numIndices = sizeof(point_sphere_indices) / sizeof(int);
			graphicsShapeIndex = m_instancingRenderer->registerShape(&point_sphere_vertices[0], numVertices, point_sphere_indices, numIndices, B3_GL_POINTS, textureId);
			break;
		}

		case SPHERE_LOD_LOW:
		{
			int numVertices = sizeof(low_sphere_vertices) / strideInBytes;
			int numIndices = sizeof(low_sphere_indices) / sizeof(int);
			graphicsShapeIndex = m_instancingRenderer->registerShape(&low_sphere_vertices[0], numVertices, low_sphere_indices, numIndices, B3_GL_TRIANGLES, textureId);
			break;
		}
		case SPHERE_LOD_MEDIUM:
		{
			int numVertices = sizeof(textured_detailed_sphere_vertices) / strideInBytes;
			int numIndices = sizeof(textured_detailed_sphere_indices) / sizeof(int);
			graphicsShapeIndex = m_instancingRenderer->registerShape(&textured_detailed_sphere_vertices[0], numVertices, textured_detailed_sphere_indices, numIndices, B3_GL_TRIANGLES, textureId);
			break;
		}
		case SPHERE_LOD_HIGH:
		default:
		{
			int numVertices = sizeof(textured_detailed_sphere_vertices) / strideInBytes;
			int numIndices = sizeof(textured_detailed_sphere_indices) / sizeof(int);
			graphicsShapeIndex = m_instancingRenderer->registerShape(&textured_detailed_sphere_vertices[0], numVertices, textured_detailed_sphere_indices, numIndices, B3_GL_TRIANGLES, textureId);
			break;
		}
	};
	return graphicsShapeIndex;
}

void GlfwApp::drawGrid(DrawGridData data)
{
	int gridSize = data.gridSize;
	float upOffset = data.upOffset;
	int upAxis = data.upAxis;
	float gridColor[4];
	gridColor[0] = data.gridColor[0];
	gridColor[1] = data.gridColor[1];
	gridColor[2] = data.gridColor[2];
	gridColor[3] = data.gridColor[3];

	int sideAxis = -1;
	int forwardAxis = -1;

	switch (upAxis)
	{
		case 1:
			forwardAxis = 2;
			sideAxis = 0;
			break;
		case 2:
			forwardAxis = 1;
			sideAxis = 0;
			break;
		default:
			b3Assert(0);
	};
	//b3Vector3 gridColor = b3MakeVector3(0.5,0.5,0.5);

	b3AlignedObjectArray<unsigned int> indices;
	b3AlignedObjectArray<b3Vector3> vertices;
	int lineIndex = 0;
	for (int i = -gridSize; i <= gridSize; i++)
	{
		{
			b3Assert(glGetError() == GL_NO_ERROR);
			b3Vector3 from = b3MakeVector3(0, 0, 0);
			from[sideAxis] = float(i);
			from[upAxis] = upOffset;
			from[forwardAxis] = float(-gridSize);
			b3Vector3 to = b3MakeVector3(0, 0, 0);
			to[sideAxis] = float(i);
			to[upAxis] = upOffset;
			to[forwardAxis] = float(gridSize);
			vertices.push_back(from);
			indices.push_back(lineIndex++);
			vertices.push_back(to);
			indices.push_back(lineIndex++);
			// m_instancingRenderer->drawLine(from,to,gridColor);
		}

		b3Assert(glGetError() == GL_NO_ERROR);
		{
			b3Assert(glGetError() == GL_NO_ERROR);
			b3Vector3 from = b3MakeVector3(0, 0, 0);
			from[sideAxis] = float(-gridSize);
			from[upAxis] = upOffset;
			from[forwardAxis] = float(i);
			b3Vector3 to = b3MakeVector3(0, 0, 0);
			to[sideAxis] = float(gridSize);
			to[upAxis] = upOffset;
			to[forwardAxis] = float(i);
			vertices.push_back(from);
			indices.push_back(lineIndex++);
			vertices.push_back(to);
			indices.push_back(lineIndex++);
			// m_instancingRenderer->drawLine(from,to,gridColor);
		}
	}

	m_instancingRenderer->drawLines(&vertices[0].x,
									gridColor,
									vertices.size(), sizeof(b3Vector3), &indices[0], indices.size(), 1);

	m_instancingRenderer->drawLine(b3MakeVector3(0, 0, 0), b3MakeVector3(1, 0, 0), b3MakeVector3(1, 0, 0), 3);
	m_instancingRenderer->drawLine(b3MakeVector3(0, 0, 0), b3MakeVector3(0, 1, 0), b3MakeVector3(0, 1, 0), 3);
	m_instancingRenderer->drawLine(b3MakeVector3(0, 0, 0), b3MakeVector3(0, 0, 1), b3MakeVector3(0, 0, 1), 3);

	//	void GLInstancingRenderer::drawPoints(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, float pointDrawSize)

	//we don't use drawPoints because all points would have the same color
	//	b3Vector3 points[3] = { b3MakeVector3(1, 0, 0), b3MakeVector3(0, 1, 0), b3MakeVector3(0, 0, 1) };
	//	m_instancingRenderer->drawPoints(&points[0].x, b3MakeVector3(1, 0, 0), 3, sizeof(b3Vector3), 6);

	m_instancingRenderer->drawPoint(b3MakeVector3(1, 0, 0), b3MakeVector3(1, 0, 0), 6);
	m_instancingRenderer->drawPoint(b3MakeVector3(0, 1, 0), b3MakeVector3(0, 1, 0), 6);
	m_instancingRenderer->drawPoint(b3MakeVector3(0, 0, 1), b3MakeVector3(0, 0, 1), 6);
}

void GlfwApp::setBackgroundColor(float red, float green, float blue)
{
	CommonGraphicsApp::setBackgroundColor(red, green, blue);
	glClearColor(m_backgroundColorRGB[0], m_backgroundColorRGB[1], m_backgroundColorRGB[2], 1.f);
}

void GlfwApp::setViewport(int width, int height)
{
	m_customViewPortWidth = width;
	m_customViewPortHeight = height;
	if (width >= 0)
	{
		glViewport(0, 0, width, height);
	}
	else
	{
		auto cam = m_instancingRenderer->getActiveCamera();
		glViewport(0, 0, m_retinaScale * cam->getScreenWidth(), m_retinaScale * cam->getScreenHeight());
	}
}

void GlfwApp::getScreenPixels(unsigned char* rgbaBuffer, int bufferSizeInBytes, float* depthBuffer, int depthBufferSizeInBytes)
{
	auto cam = m_instancingRenderer->getActiveCamera();
	int width = m_customViewPortWidth >= 0 ? m_customViewPortWidth : (int)m_retinaScale * cam->getScreenWidth();
	int height = m_customViewPortHeight >= 0 ? m_customViewPortHeight : (int)m_retinaScale * cam->getScreenHeight();

	b3Assert((width * height * 4) == bufferSizeInBytes);
	if ((width * height * 4) == bufferSizeInBytes)
	{
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgbaBuffer);
		int glstat;
		glstat = glGetError();
		b3Assert(glstat == GL_NO_ERROR);
	}
	b3Assert((width * height * sizeof(float)) == depthBufferSizeInBytes);
	if ((width * height * sizeof(float)) == depthBufferSizeInBytes)
	{
		glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer);
		int glstat;
		glstat = glGetError();
		b3Assert(glstat == GL_NO_ERROR);
	}
}

static void writeTextureToFile(int textureWidth, int textureHeight, const char* fileName, FILE* ffmpegVideo)
{
	int numComponents = 4;
	//glPixelStorei(GL_PACK_ALIGNMENT,1);

	b3Assert(glGetError() == GL_NO_ERROR);
	//glReadBuffer(GL_BACK);//COLOR_ATTACHMENT0);

	float* orgPixels = (float*)malloc(textureWidth * textureHeight * numComponents * 4);
	glReadPixels(0, 0, textureWidth, textureHeight, GL_RGBA, GL_FLOAT, orgPixels);
	//it is useful to have the actual float values for debugging purposes

	//convert float->char
	char* pixels = (char*)malloc(textureWidth * textureHeight * numComponents);
	assert(glGetError() == GL_NO_ERROR);

	for (int j = 0; j < textureHeight; j++)
	{
		for (int i = 0; i < textureWidth; i++)
		{
			pixels[(j * textureWidth + i) * numComponents] = char(orgPixels[(j * textureWidth + i) * numComponents] * 255.f);
			pixels[(j * textureWidth + i) * numComponents + 1] = char(orgPixels[(j * textureWidth + i) * numComponents + 1] * 255.f);
			pixels[(j * textureWidth + i) * numComponents + 2] = char(orgPixels[(j * textureWidth + i) * numComponents + 2] * 255.f);
			pixels[(j * textureWidth + i) * numComponents + 3] = char(orgPixels[(j * textureWidth + i) * numComponents + 3] * 255.f);
		}
	}

	if (ffmpegVideo)
	{
		fwrite(pixels, textureWidth * textureHeight * numComponents, 1, ffmpegVideo);
		//fwrite(pixels, 100,1,ffmpegVideo);//textureWidth*textureHeight*numComponents, 1, ffmpegVideo);
	}
	else
	{
		if (1)
		{
			//swap the pixels
			unsigned char tmp;

			for (int j = 0; j < textureHeight / 2; j++)
			{
				for (int i = 0; i < textureWidth; i++)
				{
					for (int c = 0; c < numComponents; c++)
					{
						tmp = pixels[(j * textureWidth + i) * numComponents + c];
						pixels[(j * textureWidth + i) * numComponents + c] =
							pixels[((textureHeight - j - 1) * textureWidth + i) * numComponents + c];
						pixels[((textureHeight - j - 1) * textureWidth + i) * numComponents + c] = tmp;
					}
				}
			}
		}
		stbi_write_png(fileName, textureWidth, textureHeight, numComponents, pixels, textureWidth * numComponents);
	}

	free(pixels);
	free(orgPixels);
}

// void GlfwApp::swapBuffer()
// {
// 	if (m_frameDumpPngFileName)
// 	{
// 		auto cam = m_instancingRenderer->getActiveCamera();
// 		int width = (int)m_retinaScale * cam->getScreenWidth();
// 		int height = (int)m_retinaScale * cam->getScreenHeight();
// 		writeTextureToFile(width,
// 						   height, m_frameDumpPngFileName,
// 						   m_ffmpegFile);
// 		m_renderTexture->disable();
// 		if (m_ffmpegFile == 0)
// 		{
// 			m_frameDumpPngFileName = 0;
// 		}
// 	}
// 	m_window->endRendering();
// 	m_window->startRendering();
// }

void GlfwApp::setMp4Fps(int fps)
{
	m_mp4Fps = fps;
}

// see also http://blog.mmacklin.com/2013/06/11/real-time-video-capture-with-ffmpeg/
void GlfwApp::dumpFramesToVideo(const char* mp4FileName)
{
	if (mp4FileName)
	{
		auto cam = m_instancingRenderer->getActiveCamera();
		int width = (int)m_retinaScale * cam->getScreenWidth();
		int height = (int)m_retinaScale * cam->getScreenHeight();
		char cmd[8192];

		sprintf(cmd,
				"ffmpeg -r %d -f rawvideo -pix_fmt rgba -s %dx%d -i - "
				"-threads 0 -y -b:v 50000k   -c:v libx264 -preset slow -crf 22 -an   -pix_fmt yuv420p -vf vflip %s",
				m_mp4Fps, width, height, mp4FileName);

		if (m_ffmpegFile)
		{
			_pclose(m_ffmpegFile);
		}
		if (mp4FileName)
		{
			m_ffmpegFile = _popen(cmd, "w");

			m_frameDumpPngFileName = mp4FileName;
		}
	}
	else
	{
		if (m_ffmpegFile)
		{
			fflush(m_ffmpegFile);
			_pclose(m_ffmpegFile);
			m_frameDumpPngFileName = 0;
		}
		m_ffmpegFile = 0;
	}
}

void GlfwApp::dumpNextFrameToPng(const char* filename)
{
	// open pipe to ffmpeg's stdin in binary write mode

	m_frameDumpPngFileName = filename;

	//you could use m_renderTexture to allow to render at higher resolutions, such as 4k or so
	if (!m_renderTexture)
	{
		auto cam = m_instancingRenderer->getActiveCamera();
		m_renderTexture = new GLRenderToTexture();
		GLuint renderTextureId;
		glGenTextures(1, &renderTextureId);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderTextureId);

		// Give an empty image to OpenGL ( the last "0" )
		//glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, g_OpenGLWidth,g_OpenGLHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
		//glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, g_OpenGLWidth,g_OpenGLHeight, 0,GL_RGBA, GL_FLOAT, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
					 cam->getScreenWidth() * m_retinaScale, cam->getScreenHeight() * m_retinaScale, 0, GL_RGBA, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_renderTexture->init(cam->getScreenWidth() * m_retinaScale, cam->getScreenHeight() * m_retinaScale, renderTextureId, RENDERTEXTURE_COLOR);
	}

	m_renderTexture->enable();
}

void GlfwApp::closeWindow()
{
	setRequestExit();
}

float GlfwApp::getTimeInSeconds()
{
	return glfwGetTime();
}

bool GlfwApp::requestedExit() const
{
	if (!m_window)
	{
		return true;
	}
	if (glfwWindowShouldClose(m_window))
	{
		return true;
	}
	return false;
	;
}

void GlfwApp::setRequestExit()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void GlfwApp::startRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glfwPollEvents();
}

void GlfwApp::endRendering()
{
	glfwSwapBuffers(m_window);
}

bool GlfwApp::isModifierKeyPressed(int key)
{
	return glfwGetKey(m_window, GLFW_KEY_LEFT_ALT);
}

int GlfwApp::getWidth() const
{
	int width;
	int height;
	glfwGetWindowSize(m_window, &width, &height);
	return width;
}

int GlfwApp::getHeight() const
{
	int width;
	int height;
	glfwGetWindowSize(m_window, &width, &height);
	return height;
}

int GlfwApp::fileOpenDialog(char* fileName, int maxFileNameLength)
{
	return 0;
}
