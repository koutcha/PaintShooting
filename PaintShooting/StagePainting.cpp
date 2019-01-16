#include "StagePainting.h"
#include "PaintMarkTexture.h"
#include "PaintBufferTexture.h"

StagePainting::StagePainting(const std::shared_ptr<PaintBufferTexture>& buffer, const std::shared_ptr<PaintMarkTexture>& texture):
	buffer(buffer),
	texture(texture)
{
}

StagePainting::~StagePainting()
{
}


void StagePainting::inputPos(float positionX, float positonZ, float scale, const Vector3f & velocity, int uvID, int teamIndex)
{
	buffer->inputPos(positionX, positonZ, scale,velocity, uvID, teamIndex);
}

void StagePainting::renderInput()
{
	buffer->renderInput(*texture);
}

void StagePainting::bindTexture(unsigned int unit)const
{
	buffer->bind(unit);
}

void StagePainting::unbind(unsigned int unit)const
{
	buffer->unbind(unit);
}

