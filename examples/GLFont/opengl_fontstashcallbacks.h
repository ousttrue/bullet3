#pragma once
#include <memory>
#include "fontstash.h"
#include "GLVBO.h"
#include "GLVAO.h"
class GLPrimitiveRenderer;

class OpenGL2RenderCallbacks : public RenderCallbacks
{
	GLPrimitiveRenderer* m_primRender2 = nullptr;
	std::shared_ptr<GLIBO> s_indexBuffer;
	std::shared_ptr<GLVAO> s_vertexArrayObject;
	std::shared_ptr<GLVBO> s_vertexBuffer;

public:
	OpenGL2RenderCallbacks(GLPrimitiveRenderer* primRender) : m_primRender2(primRender) {}
	void updateTexture(sth_texture* texture, sth_glyph* glyph, int textureWidth, int textureHeight) override;
	void render(sth_texture* texture) override;

private:
	void display2();
};

void dumpTextureToPng(int screenWidth, int screenHeight, const char* fileName);
