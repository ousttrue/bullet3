#pragma once
#include "fontstash.h"
#include "GLMesh.h"
struct PrimInternalData;

class OpenGL2RenderCallbacks : public RenderCallbacks
{
	PrimInternalData* m_data = nullptr;
	std::shared_ptr<GLMesh> m_mesh;

public:
	OpenGL2RenderCallbacks(PrimInternalData* data) : m_data(data) {}
	void updateTexture(sth_texture* texture, sth_glyph* glyph, int textureWidth, int textureHeight) override;
	void render(sth_texture* texture) override;

private:
	void display2();
};

void dumpTextureToPng(int screenWidth, int screenHeight, const char* fileName);
