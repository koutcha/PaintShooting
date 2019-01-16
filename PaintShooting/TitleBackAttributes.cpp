#include "TitleBackAttributes.h"
#include "Shader.h"


TitleBackAttributes::TitleBackAttributes(const Vector3f & color0, const Vector3f & color1):
	colors({color0,color1})
{
	
}

TitleBackAttributes::~TitleBackAttributes()
{
}

void TitleBackAttributes::setAttributes(const Shader &shader) const
{
	glUniform3fv(shader.getUniformLoc("color0"), 1, colors[0].data());
	glUniform3fv(shader.getUniformLoc("color1"), 1, colors[1].data());

}

void TitleBackAttributes::unbindAttributes() const
{

}

const std::array<Vector3f, 2> TitleBackAttributes::getColors() const
{
	return colors;
}
