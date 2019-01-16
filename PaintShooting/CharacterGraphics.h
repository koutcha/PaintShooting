#pragma once
#include <memory>
class RigidData;
class PaintActorData;
class Shape;
class MaterialPBR;
class GraphicIBL;
class PaintColorSet;
class CharacterGraphics
{
public:
	CharacterGraphics(const std::shared_ptr<Shape>& shape,const std::shared_ptr<const MaterialPBR>& meterial,const std::shared_ptr<PaintColorSet>& colorSet);
	~CharacterGraphics();
	void setRigid(const std::shared_ptr<RigidData>&);
	void setActorData(const std::shared_ptr<PaintActorData>&);
	void draw(const GraphicIBL&g);
private:
	std::shared_ptr<RigidData> rigid;
	std::shared_ptr<PaintActorData> data;
	std::shared_ptr<Shape> shape;
	std::unique_ptr<MaterialPBR> uniqueMaterial;

	std::shared_ptr<PaintColorSet> colorSet;
	std::shared_ptr<const MaterialPBR> commonMaterial;
};

