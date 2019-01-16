#include "Font.h"
#include "InitShader.h"

#include <iostream>
#include <iomanip>
#include "Shader.h"
#include "Matrix.h"
#include "Window.h"


const std::u32string Font::getBasicCharSet()
{
	//もうちょっといい方法があるはず.ループ使いたくない

	std::u32string charset;
	charset.reserve(0x7F - 0x21);
	
	//アルファベット、記号
	for (uint32_t i = 0x21; i < 0x7F; ++i)
	{
		charset += i;
	}
	return charset;
}

const std::u32string Font::getKanaCharSet()
{
	//もうちょっといい方法があるはず

	std::u32string charset;
	charset.reserve((0x3097 - 0x3041) + (0x30F7 - 0x30A1));
	//ひらがな
	for (uint32_t i = 0x3041; i < 0x3097; ++i)
	{
		charset += i;
	}
	//カタカナ
	for (uint32_t i = 0x30A1; i < 0x30F7; ++i)
	{
		charset += i;
	}
	return charset;
}

Font::Font(const char* filepath):
	shader(std::make_shared<Shader>("Shader\\Text.vert", "Shader\\text.frag"))
{

	//フリータイプのハンドル
	FT_Library ft;

	FT_Face face;

	//初期化
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init freetype library" << std::endl;
		return;
	}

	//フォントのロード
	if (FT_New_Face(ft, filepath, 0, &face)) {
		std::cerr << "Could not open Font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 72, 72);


	//バイトアラインメント制限の無効化、赤1値テクスチャだから必要
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//テクスチャとフォントのハッシュテーブル作成
	// ASCII
	for (GLubyte c = 0; c < 128; c++)
	{
		
		if (FT_Load_Char(face,c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// テクスチャの生成
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			{face->glyph->bitmap.width, face->glyph->bitmap.rows},
			{face->glyph->bitmap_left, face->glyph->bitmap_top },
			face->glyph->advance.x
		};
		characters.emplace(c, character);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "suceeded to load " << filepath << std::endl;

}

Font::Font(const char * filepath, const std::u32string & u32GlyphArray):
	shader(std::make_shared<Shader>("Shader\\Text.vert", "Shader\\text.frag"))
{

	//ft lib handle
	FT_Library ft;
	//face object handle
	FT_Face face;

	/* Initialize the FreeType2 library */
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init freetype library" << std::endl;
		return;
	}

	/* Load a font */
	//char* font = file_read(filepath);
	if (FT_New_Face(ft, filepath, 0, &face)) {
		std::cerr << "Could not open Font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 48, 48);


	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			{face->glyph->bitmap.width, face->glyph->bitmap.rows},
			{face->glyph->bitmap_left, face->glyph->bitmap_top },
			face->glyph->advance.x
		};
		characters.emplace(c, character);
	}

	loadGryph(u32GlyphArray, kana, ft, face);

	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "suceeded to load " << filepath << std::endl;
}

Font::~Font()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	
	for (auto& c : characters)
	{

		glDeleteTextures(1, &c.second.TextureID);
	}
	for (auto& u32 : kana)
	{

		glDeleteTextures(1, &u32.second.TextureID);
	}
}

void Font::loadGryph(const std::u32string & u32, std::unordered_map<char32_t, Character>& map, FT_Library & ft, FT_Face & face)
{
	
	for (int i = 0; i < u32.size(); i++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, u32[i], FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
	
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			{face->glyph->bitmap.width, face->glyph->bitmap.rows},
			{face->glyph->bitmap_left, face->glyph->bitmap_top },
			face->glyph->advance.x
		};
		map.emplace(u32[i], character);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Font::renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale,const GLfloat color[3],GLfloat width,GLfloat height) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->use();
	Matrix projection = Matrix::orthogonal(0.0f,width, 0.0f, height,-10,10);
	glUniformMatrix4fv(shader->getUniformLoc("projection"), 1, GL_FALSE,projection.data());

	glUniform3f(shader->getUniformLoc("textColor"), color[0], color[1], color[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);


	std::unordered_map<GLchar, Character>::const_iterator itr;
	for (int i = 0; i<text.size(); i++)
	{

		itr = characters.find(text[i]);
		if (itr == characters.end())
		{
			continue;
		}
		
		Character ch = itr->second;

		GLfloat xpos = x + ch.Bearing[0] * scale;
		GLfloat ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

		GLfloat w = ch.Size[0] * scale;
		GLfloat h = ch.Size[1] * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}
void Font::renderText(const std::string & text, float x, float y, float scale, const Vector3f & color, float width, float height) const
{
	renderText(text, x, y, scale, color.getArray().data(), width, height);
}

void Font::renderText(const std::string & text, float x, float y, float scale, const Vector3f & color, const Window & window) const
{
	renderText(text, x, y, scale, color.getArray().data(),window.getSize()[0],window.getSize()[1]);
}

float Font::getTextWidth(const std::string & text, float scale) const
{
	float width = 0.0f;
	std::unordered_map<GLchar, Character>::const_iterator itr;
	for (int i = 0; i < text.size(); i++)
	{

		itr = characters.find(text[i]);
		if (itr == characters.end())
		{
			continue;
		}

		Character ch = itr->second;
		width += (ch.Advance >> 6) * scale; 
	}
	return width;
}

void Font::renderText(const std::u32string & u32, float x, float y, float scale, const Vector3f & color, float width, float height) const
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->use();
	Matrix projection = Matrix::orthogonal(0.0f, width, 0.0f, height, -10, 10);
	glUniformMatrix4fv(shader->getUniformLoc("projection"), 1, GL_FALSE, projection.data());

	glUniform3f(shader->getUniformLoc("textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	// Iterate through all characters
	std::unordered_map<char32_t, Character>::const_iterator itr;

	for (int i = 0; i < u32.size(); i++)
	{

		itr = kana.find(u32[i]);
		if (itr == kana.end())
		{
			continue;
		}

		Character ch = itr->second;

		GLfloat xpos = x + ch.Bearing[0] * scale;
		GLfloat ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

		GLfloat w = ch.Size[0] * scale;
		GLfloat h = ch.Size[1] * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		x += (ch.Advance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void Font::renderText(const std::u32string & u32, float x, float y, float scale, const Vector3f & color, const Window & window) const
{
	renderText(u32, x, y, scale, color, window.getSize()[0], window.getSize()[1]);
}
