#include "opengl_fontstashcallbacks.h"
#include "GLMesh.h"
#include "GLPrimitiveRenderer.h"
#include "GLShader.h"
#include "GLVAO.h"
#include "GLVBO.h"
#include <glad/gl.h>
#include <assert.h>
#include <stb_image_write.h>
#include <memory>

void OpenGL2RenderCallbacks::display2()
{
	m_data->m_shader->use();
	float identity[16] = {1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1};
	glUniformMatrix4fv(m_data->m_viewmatUniform, 1, false, identity);
	glUniformMatrix4fv(m_data->m_projMatUniform, 1, false, identity);

	vec2 p(0.f, 0.f);  //?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
	glUniform2fv(m_data->m_positionUniform, 1, (const GLfloat*)&p);
}

void OpenGL2RenderCallbacks::updateTexture(sth_texture* texture, sth_glyph* glyph, int textureWidth, int textureHeight)
{
	assert(glGetError() == GL_NO_ERROR);

	if (glyph)
	{
		// Update texture (entire texture, could use glyph to update partial texture using glTexSubImage2D)
		GLuint* gltexture = (GLuint*)texture->m_userData;

		glBindTexture(GL_TEXTURE_2D, *gltexture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		assert(glGetError() == GL_NO_ERROR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, texture->m_texels);

		assert(glGetError() == GL_NO_ERROR);
	}
	else
	{
		if (textureWidth && textureHeight)
		{
			GLuint* texId = new GLuint;
			texture->m_userData = texId;

			//create new texture
			glGenTextures(1, texId);
			assert(glGetError() == GL_NO_ERROR);

			glBindTexture(GL_TEXTURE_2D, *texId);
			texture->m_texels = (unsigned char*)malloc(textureWidth * textureHeight);
			memset(texture->m_texels, 0, textureWidth * textureHeight);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, texture->m_texels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			assert(glGetError() == GL_NO_ERROR);

			////////////////////////////
			//create the other data
			{
				auto vertices = GLVBO::load(texture->newverts, VERT_COUNT * sizeof(Vertex), true);
				unsigned int s_indexData[INDEX_COUNT];
				for (int i = 0; i < INDEX_COUNT; i++)
				{
					s_indexData[i] = i;
				}
				auto indices = GLIBO::load(s_indexData);
				m_mesh = std::make_shared<GLMesh>(vertices, indices);
			}
		}
		else
		{
			//delete texture
			if (texture->m_userData)
			{
				GLuint* id = (GLuint*)texture->m_userData;

				glDeleteTextures(1, id);
				//delete id;
				delete id;  //texture->m_userData;
				texture->m_userData = 0;
			}
		}
	}
}

void OpenGL2RenderCallbacks::render(sth_texture* texture)
{
	display2();

	GLuint* texId = (GLuint*)texture->m_userData;

	assert(glGetError() == GL_NO_ERROR);

	glActiveTexture(GL_TEXTURE0);
	assert(glGetError() == GL_NO_ERROR);

	glBindTexture(GL_TEXTURE_2D, *texId);
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
	assert(glGetError() == GL_NO_ERROR);

	m_mesh->vbo()->upload(&texture->newverts[0].position.p[0], texture->nverts * sizeof(Vertex));

	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	int indexCount = texture->nverts;
	assert(glGetError() == GL_NO_ERROR);
	m_mesh->draw(indexCount);
	//	 glDisableVertexAttribArray(m_textureAttribute);
	glUseProgram(0);
}

void dumpTextureToPng(int textureWidth, int textureHeight, const char* fileName)
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	unsigned char* pixels = (unsigned char*)malloc(textureWidth * textureHeight);
	glReadPixels(0, 0, textureWidth, textureHeight, GL_RED, GL_UNSIGNED_BYTE, pixels);
	//swap the pixels
	unsigned char* tmp = (unsigned char*)malloc(textureWidth);
	for (int j = 0; j < textureHeight; j++)
	{
		pixels[j * textureWidth + j] = 255;
	}
	if (0)
	{
		for (int j = 0; j < textureHeight / 2; j++)
		{
			for (int i = 0; i < textureWidth; i++)
			{
				tmp[i] = pixels[j * textureWidth + i];
				pixels[j * textureWidth + i] = pixels[(textureHeight - j - 1) * textureWidth + i];
				pixels[(textureHeight - j - 1) * textureWidth + i] = tmp[i];
			}
		}
	}

	int comp = 1;  //1=Y
	stbi_write_png(fileName, textureWidth, textureHeight, comp, pixels, textureWidth);

	free(pixels);
}
