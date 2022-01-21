#include "GlfwApp.h"
#include "GLRenderToTexture.h"
#include "ShapeData.h"
#include <Bullet3Common/b3Quaternion.h>
#include <stb_image_write.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <assert.h>
#include <stdexcept>

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

	m_renderer = m_instancingRenderer;

	m_instancingRenderer->init();
	m_instancingRenderer->resize(width, height);
	b3Assert(glGetError() == GL_NO_ERROR);

	m_instancingRenderer->InitShaders();
	return true;
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
