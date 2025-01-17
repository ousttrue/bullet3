#include "GLShader.h"
#include "OpenGLInclude.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vcruntime.h>

// Load the shader from the source text
void gltLoadShaderSrc(const char *szShaderSrc, GLuint shader)
{
	GLchar *fsStringPtr[1];

	fsStringPtr[0] = (GLchar *)szShaderSrc;
	glShaderSource(shader, 1, (const GLchar **)fsStringPtr, NULL);
}

GLShader::~GLShader()
{
	glDeleteProgram(m_program);
}

std::shared_ptr<GLShader> GLShader::load(const char *szVertexProg, const char *szFragmentProg)
{
	assert(glGetError() == GL_NO_ERROR);

	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader;
	GLuint hReturn = 0;
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	gltLoadShaderSrc(szVertexProg, hVertexShader);
	gltLoadShaderSrc(szFragmentProg, hFragmentShader);

	// Compile them
	glCompileShader(hVertexShader);
	assert(glGetError() == GL_NO_ERROR);

	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE)
	{
		char temp[256] = "";
		glGetShaderInfoLog(hVertexShader, 256, NULL, temp);
		fprintf(stderr, "Compile failed:\n%s\n", temp);
		assert(0);
		return 0;
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return {};
	}

	assert(glGetError() == GL_NO_ERROR);

	glCompileShader(hFragmentShader);
	assert(glGetError() == GL_NO_ERROR);

	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE)
	{
		char temp[256] = "";
		glGetShaderInfoLog(hFragmentShader, 256, NULL, temp);
		fprintf(stderr, "Compile failed:\n%s\n", temp);
		assert(0);
		exit(EXIT_FAILURE);
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return {};
	}

	assert(glGetError() == GL_NO_ERROR);

	// Check for errors

	// Link them - assuming it works...
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);

	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);

	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if (testVal == GL_FALSE)
	{
		GLsizei maxLen = 4096;
		GLchar infoLog[4096];
		GLsizei actualLen;

		glGetProgramInfoLog(hReturn,
							maxLen,
							&actualLen,
							infoLog);

		printf("Warning/Error in GLSL shader:\n");
		printf("%s\n", infoLog);
		glDeleteProgram(hReturn);
		return {};
	}

	return std::shared_ptr<GLShader>(new GLShader(hReturn));
}

int GLShader::getUniformLocation(const char *name)
{
	return glGetUniformLocation(m_program, name);
}

int GLShader::getAttributeLocation(const char *name)
{
	return glGetAttribLocation(m_program, name);
}

void GLShader::use()
{
	glUseProgram(m_program);
}

void GLShader::unuse()
{
	glUseProgram(0);
}

uint32_t GLUniformVarable::getLocation(uint32_t program) const
{
	if (m_location == -1)
	{
		m_location = glGetUniformLocation(program, m_name.c_str());
		assert(m_location != -1);
	}
	return m_location;
}
void GLUniformVarable::setMat4(const float *p) const
{
	assert(m_location != -1);
	glUniformMatrix4fv(m_location, 1, false, p);
}
void GLUniformVarable::setFloat2(const float *p) const
{
	assert(m_location != -1);
	glUniform2fv(m_location, 1, p);
}

uint32_t GLVertexAttribute::getLocation(uint32_t program) const
{
	if (m_location == -1)
	{
		m_location = glad_glGetAttribLocation(program, m_name.c_str());
		assert(m_location != -1);
	}
	return m_location;
}

void GLVertexAttribute::enable(unsigned int float_count, size_t stride, size_t offset) const
{
	assert(m_location != -1);
	glEnableVertexAttribArray(m_location);
	glVertexAttribPointer(m_location, float_count, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}
