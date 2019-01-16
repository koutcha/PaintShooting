#pragma once
#include <memory>
#include <string>
#include "Vector3.h"
class PaintBufferTexture;
class PaintMarkTexture;
class StageData;
class StageAttributeSetter;
class StagePainting
{
public:
	StagePainting(const std::shared_ptr<PaintBufferTexture>&,const std::shared_ptr<PaintMarkTexture>&);
	~StagePainting();
	//void inputPos(float positionX, float positonZ, float scale, int uvID, int teamIndex);
	void inputPos(float positionX, float positonZ, float scale,const Vector3f& velocity, int uvID, int teamIndex);
	//void inputPos(float positionX, float positonZ, float scale,const Vector3f& velocity, int teamIndex);

	void renderInput();
	void bindTexture(unsigned int unit)const;
	void unbind(unsigned int unit)const;
private:
	std::shared_ptr<PaintBufferTexture> buffer;
	std::shared_ptr<PaintMarkTexture> texture;
};

