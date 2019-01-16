#define _USE_MATH_DEFINES
#include "StageGraphics.h"
#include "StageData.h"
#include "Vector3.h"
#include "TexturePackPBR.h"
#include "GraphicIBL.h"
#include "StageAttributeSetter.h"
#include "Shader.h"
#include "StagePainting.h"
#include "StageObstacle.h"

#include "SolidShapeIndex.h"

#include "TextureRenderer2D.h"

#include "Window.h"
#include "GL/glew.h"

StageGraphics::StageGraphics(const std::shared_ptr<Shape>& shape):
	shape(shape),
	shader(std::make_shared<Shader>("Shader\\common.vert","Shader\\StageIBL.frag")),
	pbr(std::make_shared<TexturePackPBR>("Resource\\Image\\debug")),
	wall(SolidShapeIndex::createVerticalPlane(1.0,1.0))
{

}

StageGraphics::StageGraphics(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Shader>& shader):
	shape(shape),
	shader(shader),
	pbr(std::make_shared<TexturePackPBR>("Resource\\Image\\debug"))
{
}

StageGraphics::~StageGraphics()
{
}

void StageGraphics::setStageData(const std::shared_ptr<StageData>& stageData)
{
	data = stageData;
}

void StageGraphics::setStagePainting(const std::shared_ptr<StagePainting>& painter)
{
	this->painter = painter;
}

void StageGraphics::setStageAttributeSetter(const std::shared_ptr<StageAttributeSetter>& setter)
{
	this->atteributeSetter = setter;
}

void StageGraphics::renderInput()
{
	painter->renderInput();
}

void StageGraphics::draw(const GraphicIBL & g)
{
	glEnable(GL_CULL_FACE);
	//ï«ÇÃÉÇÉfÉãçsóÒ
	stagePlaneMatrix = Matrix::translate(Vector3f(0, -data->getScale(), 0))*Matrix::scale(Vector3f(data->getScale(), data->getScale(), data->getScale()));
	Matrix wallMat = Matrix::translate(0, data->getWallHeight() / 2.0, -data->getScale())*Matrix::scale(data->getScale(), data->getWallHeight() / 2.0, 1.0);
	//-z
	wallMatrix[0] = wallMat;
	//-x
	wallMatrix[1] = Matrix::rotateY(1.5f*M_PI)*wallMat;
	//z
	wallMatrix[2] = Matrix::rotateY(1.0f*M_PI)*wallMat;
	//+x
	wallMatrix[3] = Matrix::rotateY(0.5f*M_PI)*wallMat;

	Matrix  obstacleMat;
	//è·äQï®
	for (const auto& obstacle :data->getObstales())
	{
		obstacleMat = Matrix::translate(obstacle.getPosition())*Matrix::scale(obstacle.getScale());
		g.draw(*obstacle.getShape(),obstacleMat,*pbr);
	}

	//ï«
	for (const auto& wallM : wallMatrix)
	{
		g.draw(*wall, wallM, *pbr);
	}
	//è∞
	g.drawWithAdditonalAttribute(*shape, stagePlaneMatrix, *pbr, *shader, *atteributeSetter);
	glDisable(GL_CULL_FACE);

}

