#pragma once
#include<memory>
#include <string>
#include <unordered_map>

#include "Vector3.h"
#include "GL\glew.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Window;
class Shader;
class Font {


	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		GLfloat Size[2];    // Size of glyph
		GLfloat Bearing[2];  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};


	std::unordered_map<GLchar, Character> characters;
	std::unordered_map<char32_t, Character> kana;

	//GLuint program;
	std::shared_ptr<Shader> shader;
	//フォントレンダリングのための矩形の頂点データ
	GLuint vbo,vao;
	//std::unique_ptr<SolidShape> shape;
public:
	//アルファベット、記号
	static const std::u32string getBasicCharSet();
	//ひらがな,カタカナ
	static const std::u32string getKanaCharSet();
	//ASCIIのみロード
	Font(const char* filepath);
	//ASCII以外の文字をロードしたいときは第二引数に文字列リストをわたす
	Font(const char* filepath, const std::u32string& u32GlyphArray);

	Font(const Font&) = delete;
	void operator=(const Font&) = delete;
	//例　あいうえお　をロードしたければ　U"あいうえお"をわたす
	~Font();
	static void loadGryph(const std::u32string& u32,std::unordered_map<char32_t,Character>& map,FT_Library& ft, FT_Face& face);
	//ASCII
	void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const GLfloat color[3], GLfloat width, GLfloat height) const;
	//ASCII
	void renderText(const std::string& text, float x, float y, float scale, const Vector3f& color, float width, float height) const;
	void renderText(const std::string& text, float x, float y, float scale, const Vector3f& color, const Window& window) const;
	//getTextPixcelWidth
	float getTextWidth(const std::string& text,float scale)const;

	//UTF32
	void renderText(const std::u32string& u32, float x, float y, float sclae, const Vector3f&color, float width, float height) const;
	void renderText(const std::u32string& u32, float x, float y, float sclae, const Vector3f&color, const Window& window) const;


};
