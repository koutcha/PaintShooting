#pragma once
#include <string>
#include <memory>
class Texture;
class PaintMarkTexture
{
public:
	PaintMarkTexture(const std::string& filePath,float divisionNumber);
	~PaintMarkTexture();
	PaintMarkTexture(const PaintMarkTexture&) = delete;
	float getDivisionNumber()const;
	void bind(unsigned int unit)const;
private:
	std::unique_ptr<Texture> texture;
	float divisionNumber;
};

