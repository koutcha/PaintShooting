#pragma once
#include <string>
class LUTTextureForIBL
{
public:
	LUTTextureForIBL();
	~LUTTextureForIBL();
	void bind(unsigned int i)const;
	void unbind(unsigned int i)const;
private:
	static const std::string shaderPath;
	unsigned int texture;
};

