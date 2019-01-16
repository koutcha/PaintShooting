#pragma once
#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& filename);
	~Texture();
	void bind(unsigned int unit)const;
	void unbind(unsigned int unit)const;
	void reset(const std::string& filename);
private:
	void loadTexture(const std::string& filename);
	GLuint textureID;
};