#include "CharacterGraphics.h"
#include "Matrix.h"
#include "MaterialPBR.h"
#include "RigidData.h"
#include "GraphicIBL.h"
#include "PaintActorData.h"
#include "PaintColorSet.h"
CharacterGraphics::CharacterGraphics(const std::shared_ptr<Shape>& shape, const std::shared_ptr<const MaterialPBR>& commonMaterial,const std::shared_ptr<PaintColorSet>& colorset):
	commonMaterial(commonMaterial),
	uniqueMaterial(std::make_unique<MaterialPBR>(*commonMaterial)),
	shape(shape),
	colorSet(colorset)
{

}

CharacterGraphics::~CharacterGraphics()
{
}

void CharacterGraphics::setRigid(const std::shared_ptr<RigidData>& rigid)
{
	this->rigid = rigid;
}

void CharacterGraphics::setActorData(const std::shared_ptr<PaintActorData>& data)
{
	this->data = data;
}

void CharacterGraphics::draw(const GraphicIBL & g)
{
	Matrix m = Matrix::translate(rigid->getPosition())*Matrix::rotate(rigid->getRotation())*Matrix::scale(rigid->getScale());
	//Ž€‚ñ‚Å‚½‚ç•‚­
	if (data->getPrimalState() == PaintActorPrimalState::INACTIVE)
	{
		uniqueMaterial->setAlbedo(Vector3f(0, 0, 0));
	}
	else
	{
		uniqueMaterial->setAlbedo(colorSet->getColor(data->getTeam()));
	}
	g.draw(*shape, m,*uniqueMaterial);
}


