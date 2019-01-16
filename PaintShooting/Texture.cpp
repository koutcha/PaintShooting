#include "Texture.h"
#include <cassert>
#include <iostream>

#include "stb_image.h"

Texture::Texture():
	textureID(0)
{
}

Texture::Texture(const std::string & filename):
	textureID(0)
{
	loadTexture(filename);
}

Texture::~Texture()
{
	if (textureID != 0)
	{
		glDeleteTextures(1, &textureID);
	}
	
}



void Texture::bind(unsigned int unit)const
{
	if (textureID == 0)
	{
		return;
	}
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::unbind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::reset(const std::string & filename)
{
	if (textureID != 0)
	{
		glDeleteTextures(1, &textureID);
		
	}
	
	loadTexture(filename);

}

void Texture::loadTexture(const std::string& filename)
{
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (imageData == nullptr)
		std::cerr << "can't file open:" << filename << std::endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(imageData);
}




