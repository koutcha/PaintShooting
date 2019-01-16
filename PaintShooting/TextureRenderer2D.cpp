#include "TextureRenderer2D.h"
#include "GL/glew.h"
#include "Matrix.h"
#include "Shader.h"
#include "AdditionalAttributeSetter.h"
const std::string TextureRenderer2D::shaderPath("Shader\\textureRenderer");
TextureRenderer2D::TextureRenderer2D():
	shader(std::make_unique<Shader>(shaderPath+".vert",shaderPath+".frag"))
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
}


TextureRenderer2D::~TextureRenderer2D()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void TextureRenderer2D::renderTexture(unsigned int textureUnit, float x, float y, float width, float height, float windowWidth, float windowHeight) const
{
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Matrix projection = Matrix::orthogonal(0.0f, windowWidth, 0.0f, windowHeight, -10, 10);
	shader->use();
	glUniformMatrix4fv(shader->getUniformLoc("projection"), 1, GL_FALSE, projection.data());

	glUniform1i(shader->getUniformLoc("tex"), textureUnit);

	glBindVertexArray(vao);
	
	GLfloat vertices[6][4] = {
		{ x,     y + height,   0.0, 0.0 },
		{ x,     y,       0.0, 1.0 },
		{ x + width, y,       1.0, 1.0 },

		{ x,     y + height,   0.0, 0.0 },
		{ x + width, y,       1.0, 1.0 },
		{ x + width, y + height,   1.0, 0.0 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void TextureRenderer2D::renderTexture(unsigned int textureUnit, float x, float y, float width, float height, float windowWidth, float windowHeight, const Shader & subShader, const AdditionalAttributeSetter& setter)const
{
	glViewport(0, 0, windowWidth, windowHeight);
	Matrix projection = Matrix::orthogonal(0.0f, windowWidth, 0.0f, windowHeight, -10, 10);
	subShader.use();
	setter.setAttributes(subShader);
	glUniformMatrix4fv(subShader.getUniformLoc("projection"), 1, GL_FALSE, projection.data());

	glUniform1i(subShader.getUniformLoc("tex"), textureUnit);

	glBindVertexArray(vao);
	
	GLfloat vertices[6][4] = {
		{ x,     y + height,   0.0, 0.0 },
		{ x,     y,       0.0, 1.0 },
		{ x + width, y,       1.0, 1.0 },

		{ x,     y + height,   0.0, 0.0 },
		{ x + width, y,       1.0, 1.0 },
		{ x + width, y + height,   1.0, 0.0 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	setter.unbindAttributes();
}

void TextureRenderer2D::renderTextureNotBlending(unsigned int textureUnit, float x, float y, float width, float height, float windowWidth, float windowHeight) const
{
	glViewport(0, 0, windowWidth, windowHeight);
	Matrix projection = Matrix::orthogonal(0.0f, windowWidth, 0.0f, windowHeight, -10, 10);
	shader->use();
	glUniformMatrix4fv(shader->getUniformLoc("projection"), 1, GL_FALSE, projection.data());

	glUniform1i(shader->getUniformLoc("tex"), textureUnit);

	glBindVertexArray(vao);

	GLfloat vertices[6][4] = {
		{ x,     y + height,   0.0, 0.0 },
		{ x,     y,       0.0, 1.0 },
		{ x + width, y,       1.0, 1.0 },

		{ x,     y + height,   0.0, 0.0 },
		{ x + width, y,       1.0, 1.0 },
		{ x + width, y + height,   1.0, 0.0 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}
