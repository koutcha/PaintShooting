#pragma once
#include <memory>
#include <array>
#include "Matrix.h"

class StageData;
class Shape;
class GraphicIBL;
class TexturePackPBR;
class PaintMarkTexture;
class Shader;
class StageAttributeSetter;
class StagePainting;

class WallGraphicsData;

class TextureRenderer2D;

class Window;

class StageGraphics
{
public:
	StageGraphics(const std::shared_ptr<Shape>& shape);
	StageGraphics(const std::shared_ptr<Shape>&,const std::shared_ptr<Shader>&);
	~StageGraphics();
	void setStageData(const std::shared_ptr<StageData>& stageData);
	void setStagePainting(const std::shared_ptr<StagePainting>& painter);
	void setStageAttributeSetter(const std::shared_ptr<StageAttributeSetter>& setter);
	void renderInput();
	void draw(const GraphicIBL& g);
private:
	Matrix stagePlaneMatrix;
	std::array<Matrix,4> wallMatrix;

	std::shared_ptr<StageData> data;
	std::shared_ptr<Shape> shape;

	std::shared_ptr<TexturePackPBR> pbr;
	
	std::shared_ptr<Shader> shader;
	std::shared_ptr<StagePainting> painter;
	std::shared_ptr<StageAttributeSetter> atteributeSetter;

	std::shared_ptr<Shape> wall;
	
};

