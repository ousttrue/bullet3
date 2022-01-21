#pragma once
#include "fontstash.h"
class GLPrimitiveRenderer;

struct OpenGL2RenderCallbacks : public RenderCallbacks
{
	GLPrimitiveRenderer* m_primRender2;
	OpenGL2RenderCallbacks(GLPrimitiveRenderer* primRender);
	void updateTexture(sth_texture* texture, sth_glyph* glyph, int textureWidth, int textureHeight) override;
	void render(sth_texture* texture) override;

private:
	void display2();
};

void dumpTextureToPng(int screenWidth, int screenHeight, const char* fileName);
