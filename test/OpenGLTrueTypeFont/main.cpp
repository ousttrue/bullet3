#include <GlfwApp.h>
#include <OpenGLInclude.h>
#include <opengl_fontstashcallbacks.h>
#include <Bullet3Common/b3Quaternion.h>
#include <Bullet3Common/b3CommandLineArgs.h>
#include <GLShader.h>
#include <GLVAO.h>
#include <GLVBO.h>
#include <GLPrimInternalData.h>
#include <GLPrimitiveRenderer.h>
#include <OpenSans.h>

// bool printStats = false;
bool pauseSimulation = false;
// bool shootObject = false;

// int m_glutScreenWidth;
// int m_glutScreenHeight;

bool useInterop = false;

static PrimInternalData sData;

// /*GLuint sData.m_texturehandle;
// GLuint sData.m_shaderProg;
// GLint m_positionUniform;
// GLint m_colourAttribute, m_positionAttribute,m_textureAttribute;
// GLuint m_vertexArrayObject,m_vertexBuffer;
// GLuint  m_indexBuffer;
// */

void loadBufferData()
{
	Vertex vertexData[4] = {
		{vec4(-0.5, -0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.0078125, 0.015625)},
		{vec4(-0.5, 0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.101562, 0.015625)},
		{vec4(0.5, 0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.101562, 0.105469)},
		{vec4(0.5, -0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.0078125, 0.105469)}};
	sData.m_vertexArrayObject = GLVAO::create();
	sData.m_vertexArrayObject->bind();
	sData.m_vertexBuffer = GLVBO::load(vertexData, sizeof(vertexData), false);

	unsigned int indexData[6] = {0, 1, 2, 0, 2, 3};
	sData.m_indexBuffer = GLIBO::load(indexData, sizeof(indexData));

	glEnableVertexAttribArray(sData.m_positionAttribute);
	glEnableVertexAttribArray(sData.m_colourAttribute);
	auto err = glGetError();
	b3Assert(err == GL_NO_ERROR);
	glEnableVertexAttribArray(sData.m_textureAttribute);

	// Vertex vertexDataOrg[4] = {
	// 	{vec4(-0.5, -0.5, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec2(0, 0)},
	// 	{vec4(-0.5, 0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0, 1)},
	// 	{vec4(0.5, 0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(1, 1)},
	// 	{vec4(0.5, -0.5, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(1, 0)}};

	// Vertex vertexData2[4] = {
	// 	{vec4(0, 0.901042, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.0078125, 0.015625)},
	// 	{vec4(0.0234375, 0.901042, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.101562, 0.015625)},
	// 	{vec4(0.0234375, 0.871094, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.101562, 0.105469)},
	// 	{vec4(0., 0.871094, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec2(0.0078125, 0.105469)}};

	glVertexAttribPointer(sData.m_positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
	glVertexAttribPointer(sData.m_colourAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(vec4));
	glVertexAttribPointer(sData.m_textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(vec4) + sizeof(vec4)));
	err = glGetError();
	b3Assert(err == GL_NO_ERROR);
}

void initTestTexture()
{
	// 	//	glEnable(GL_TEXTURE_2D);
	// 	glGenTextures(1, (GLuint*)&sData.m_texturehandle);

	// 	GLint err = glGetError();
	// 	b3Assert(err == GL_NO_ERROR);

	// 	glBindTexture(GL_TEXTURE_2D, sData.m_texturehandle);

	// 	err = glGetError();
	// 	b3Assert(err == GL_NO_ERROR);

	// 	err = glGetError();
	// 	b3Assert(err == GL_NO_ERROR);

	// 	int width = 256;
	// 	int height = 256;
	// 	unsigned char* image = (unsigned char*)malloc(width * height);
	// 	memset(image, 0, width * height);
	// 	for (int i = 0; i < width; i++)
	// 	{
	// 		for (int j = 0; j < height; j++)
	// 		{
	// 			if (i == j)
	// 				image[i + width * j] = 0;
	// 			else
	// 				image[i + width * j] = 255;
	// 		}
	// 	}
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, image);

	// 	err = glGetError();
	// 	b3Assert(err == GL_NO_ERROR);

	// 	glGenerateMipmap(GL_TEXTURE_2D);

	// 	err = glGetError();
	// 	b3Assert(err == GL_NO_ERROR);

	// 	free(image);
}

static const char* vertexShader =
	"#version 150   \n"
	"\n"
	"uniform vec2 p;\n"
	"\n"
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
	"	gl_Position = vec4(p.x+position.x, p.y+position.y,0.f,1.f);\n"
	"	texuvV=texuv;\n"
	"}\n";

static const char* fragmentShader =
	"#version 150\n"
	"\n"
	"in vec4 colourV;\n"
	"out vec4 fragColour;\n"
	"in vec2 texuvV;\n"
	"\n"
	"uniform sampler2D Diffuse;\n"
	"\n"
	"void main(void)\n"
	"{\n"
	"	vec4 texcolorred = texture(Diffuse,texuvV);\n"
	"//	vec4 texcolor = vec4(texcolorred.x,texcolorred.x,texcolorred.x,texcolorred.x);\n"
	"	vec4 texcolor = vec4(1,1,1,texcolorred.x);\n"
	"\n"
	"    fragColour = colourV*texcolor;\n"
	"}\n";

void loadShader()
{
	sData.m_shaderProg = GLShader::load(vertexShader, fragmentShader);
	sData.m_positionUniform = sData.m_shaderProg->getUniformLocation("p");
	if (sData.m_positionUniform < 0)
	{
		b3Assert(0);
	}
	sData.m_colourAttribute = sData.m_shaderProg->getAttributeLocation("colour");
	if (sData.m_colourAttribute < 0)
	{
		b3Assert(0);
	}
	sData.m_positionAttribute = sData.m_shaderProg->getAttributeLocation("position");
	if (sData.m_positionAttribute < 0)
	{
		b3Assert(0);
	}
	sData.m_textureAttribute = sData.m_shaderProg->getAttributeLocation("texuv");
	if (sData.m_textureAttribute < 0)
	{
		b3Assert(0);
	}
}

void display()
{
	GLint err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	const float timeScale = 0.008f;

	sData.m_shaderProg->use();
	sData.m_vertexBuffer->bind();
	sData.m_vertexArrayObject->bind();

	err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	//  glBindTexture(GL_TEXTURE_2D,sData.m_texturehandle);
	// 	err = glGetError();
	// 	b3Assert(err == GL_NO_ERROR);

	vec2 p(0.f, 0.f);  //?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
	glUniform2fv(sData.m_positionUniform, 1, (const GLfloat*)&p);

	err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	glEnableVertexAttribArray(sData.m_positionAttribute);
	err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	glEnableVertexAttribArray(sData.m_colourAttribute);
	err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	glEnableVertexAttribArray(sData.m_textureAttribute);

	glVertexAttribPointer(sData.m_positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
	glVertexAttribPointer(sData.m_colourAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(vec4));
	glVertexAttribPointer(sData.m_textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(vec4) + sizeof(vec4)));
	err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	sData.m_indexBuffer->bind();

	// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	int indexCount = 6;
	err = glGetError();
	b3Assert(err == GL_NO_ERROR);

	// glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	err = glGetError();
	b3Assert(err == GL_NO_ERROR);
}

const char* fileName = "../../bin/1000 stack.bullet";
void Usage()
{
	printf("\nprogram.exe [--pause_simulation=<0 or 1>] [--load_bulletfile=test.bullet] [--enable_interop=<0 or 1>] [--enable_gpusap=<0 or 1>] [--enable_convexheightfield=<0 or 1>] [--enable_static=<0 or 1>] [--x_dim=<int>] [--y_dim=<num>] [--z_dim=<int>] [--x_gap=<float>] [--y_gap=<float>] [--z_gap=<float>]\n");
};

int main(int argc, char* argv[])
{
	// GLint err;
	b3CommandLineArgs args(argc, argv);
	if (args.CheckCmdLineFlag("help"))
	{
		Usage();
		return 0;
	}

	args.GetCmdLineArgument("enable_interop", useInterop);
	printf("useInterop=%d\n", useInterop);

	args.GetCmdLineArgument("pause_simulation", pauseSimulation);
	printf("pause_simulation=%d\n", pauseSimulation);

	char* tmpfile = 0;
	args.GetCmdLineArgument("load_bulletfile", tmpfile);
	if (tmpfile)
		fileName = tmpfile;

	printf("load_bulletfile=%s\n", fileName);

	int width = 700;
	int height = 512;
	printf("\n");

	{
		GlfwApp app;
		auto window = app.createWindow({1600, 1200, "title"});
		if (!window)
		{
			return 1;
		}

		loadShader();

		loadBufferData();

		initTestTexture();

		window->startRendering();
		window->endRendering();

		auto err = glGetError();
		b3Assert(err == GL_NO_ERROR);

		//	render.InitShaders();

		//	render.writeTransforms();

		window->runMainLoop();

		//	window->setMouseCallback(b3DefaultMouseCallback);
		//	window->setKeyboardCallback(b3DefaultKeyboardCallback);
		//  window->setWheelCallback(b3DefaultWheelCallback);

		err = glGetError();
		b3Assert(err == GL_NO_ERROR);

		int done;
		int datasize;

		float sx, sy, dx, dy, lh;

		GLuint texture;

		int fontTextureWidth = 512;
		int fontTextureHeight = 512;

		auto primRenderer = new GLPrimitiveRenderer();
		auto renderCallbacks = new OpenGL2RenderCallbacks(primRenderer);

		err = glGetError();
		b3Assert(err == GL_NO_ERROR);

		// Load the first truetype font from memory (just because we can).
#ifdef _WIN32
		const char* fontPath = "../../bin/";
#else
		const char* fontPath = "./";
#endif

		char fullFontFileName[1024];
		sprintf(fullFontFileName, "%s%s", fontPath, "DroidSerif-Regular.ttf");  //cour.ttf");//times.ttf");//DroidSerif-Regular.ttf");
		//sprintf(fullFontFileName,"%s%s",fontPath,"arial.ttf");//cour.ttf");//times.ttf");//DroidSerif-Regular.ttf");

		FontStash stash(fontTextureWidth, fontTextureHeight, renderCallbacks);
		int droidRegular = stash.add_font_from_memory(OpenSansData);
		if (!droidRegular)
		{
			printf("error!\n");
			return 1;
		}

		while (!window->requestedExit())
		{
			window->startRendering();

			GLint err = glGetError();
			b3Assert(err == GL_NO_ERROR);

			// glClearColor(0.5f,0.5f,0.5f,1.f);

			err = glGetError();
			b3Assert(err == GL_NO_ERROR);

			window->startRendering();

			err = glGetError();
			b3Assert(err == GL_NO_ERROR);

			glClearColor(1, 1, 1, 1);  //.4, .4, 0.4, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//display();

			err = glGetError();
			b3Assert(err == GL_NO_ERROR);

			if (1)
			{
				B3_PROFILE("font stash rendering");
				// Update and render
				glEnable(GL_BLEND);
				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				glDisable(GL_DEPTH_TEST);
				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				//glColor4ub(255,0,0,255);

				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				glEnable(GL_BLEND);

				err = glGetError();
				b3Assert(err == GL_NO_ERROR);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				sx = 0;
				sy = height;

				stash.begin_draw();

				display();

				dx = sx;
				dy = sy;
				static int once = 0;

				//sth_draw_text(stash, droidRegular,12.f, dx, dy-50, "How does this OpenGL True Type font look? ", &dx,width,height);
				int spacing = 512;
				if (1)
					for (int i = 20; i <= 110; i += 12)
					{
						char txt[512];
						sprintf(txt, "%d. The quick brown fox jumped over the lazy dog. 1234567890", i);
						stash.draw_text(droidRegular, i, 10, dy - spacing, txt, &dx, width, height);
						spacing -= i;
					}

				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				// if (0)
				// 	for (int i = 0; i < 1; i++)
				// 	{
				// 		dx = sx;
				// 		if (once != 1)
				// 		{
				// 			//need to save this file as UTF-8 without signature, codepage 650001 in Visual Studio
				// 			err = glGetError();
				// 			b3Assert(err == GL_NO_ERROR);

				// 			//sth_draw_text(stash, droidJapanese,16.f, dx, dy-36, (const char*) "\xE7\xA7\x81\xE3\x81\xAF\xE3\x82\xAC\xE3\x83\xA9\xE3\x82\xB9\xE3\x82\x92\xE9\xA3\x9F\xE3\x81\xB9\xE3\x82\x89\xE3\x82\x8C\xE3\x81\xBE\xE3\x81\x99\xE3\x80\x82",&dx,
				// 			//              width,height);//はabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=?/\][{}.,<>`~@#$%^", &dx);
				// 			//				sth_draw_text(stash, droidJapanese,32.f, dx, dy, (const char*) "私はガラスを食べられます。それは私を傷つけません。",&dx);//はabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=?/\][{}.,<>`~@#$%^", &dx);

				// 			dx = sx;

				// 			err = glGetError();
				// 			b3Assert(err == GL_NO_ERROR);
				// 			sth_flush_draw(stash);
				// 			dx = 0;
				// 			sth_draw_text(stash, droidRegular, 14.f, dx, dy - 80, "How does this OpenGL True Type font look? ", &dx, width, height);
				// 			dx = 0;
				// 			dy -= 30;

				// 			//sth_draw_text(stash, droidRegular,16.f, dx, dy-80, "Profile How does this OpenGL True Type font look? ", &dx,width,height);
				// 			dx = 0;
				// 			dy -= 30;

				// 			sth_draw_text(stash, droidRegular, 16.f, dx, dy - 80, "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890", &dx, width, height);
				// 			dx = 0;
				// 			dy -= 30;
				// 			sth_draw_text(stash, droidRegular, 16.f, dx, dy - 80, "!@#$%^abcdefghijklmnopqrstuvwxyz", &dx, width, height);

				// 			dx = 0;
				// 			//	sth_draw_text(stash, droidRegular,16.f, dx, dy-42, "aph OpenGL Profile aCABCabdabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^", &dx,width,height);
				// 			//sth_draw_text(stash, droidRegular,16.f, dx, dy-42, "aph OpenGL Profile aCABCabdabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^", &dx,width,height);

				// 			sth_flush_draw(stash);
				// 			err = glGetError();
				// 			b3Assert(err == GL_NO_ERROR);
				// 		}
				// 		else
				// 		{
				// 			dx = sx;
				// 			dy = height;

				// 			err = glGetError();
				// 			b3Assert(err == GL_NO_ERROR);
				// 			sth_draw_texture(stash, droidRegular, 16.f, 0, 0, width, height, "a", &dx, color);
				// 			err = glGetError();
				// 			b3Assert(err == GL_NO_ERROR);

				// 			dumpTextureToPng(fontTextureWidth, fontTextureHeight, "newPic.png");
				// 		}
				// 		once++;
				// 	}
				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				stash.end_draw();

				glEnable(GL_DEPTH_TEST);
				err = glGetError();
				b3Assert(err == GL_NO_ERROR);

				//glFinish();
			}
			err = glGetError();
			b3Assert(err == GL_NO_ERROR);

			window->endRendering();

			err = glGetError();
			b3Assert(err == GL_NO_ERROR);

			{
				glFinish();
			}

			static bool printStats = true;

			if (printStats && !pauseSimulation)
			{
				static int count = 0;
				count--;
				if (count < 0)
				{
					count = 100;
					//				b3ProfileManager::dumpAll();
					//printStats  = false;
				}
				else
				{
					//				printf(".");
				}
			}

			err = glGetError();
			b3Assert(err == GL_NO_ERROR);
		}
	}

	return 0;
}
