#include "PaintMarkTexture.h"
#include "Texture.h"




PaintMarkTexture::PaintMarkTexture(const std::string & filePath, float divisionNumber):
	texture(std::make_unique<Texture>(filePath)),
	divisionNumber(divisionNumber)
{
}

PaintMarkTexture::~PaintMarkTexture()
{
}

float PaintMarkTexture::getDivisionNumber() const
{
	return divisionNumber;
}


void PaintMarkTexture::bind(unsigned int unit) const
{
	texture->bind(unit);
}
