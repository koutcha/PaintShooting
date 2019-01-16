#pragma once
#include <memory>
#include <vector>
class StageData;
class StagePhysics;
class StageGraphics;
class StagePainting;

class WallGrahicsData;

class PaintMarkTexture;
class PaintBufferTexture;
class StageAttributeSetter;
class PaintColorSet;


class Stage
{
public:
	Stage(float stageScale, const std::shared_ptr<StagePhysics>& physics, const std::shared_ptr<StageGraphics>& graphics,const std::shared_ptr<PaintBufferTexture>& paintBuffer ,const std::shared_ptr<PaintColorSet>& colorset);
	~Stage();

private:
	std::shared_ptr<StageData> data;
	std::shared_ptr<StagePhysics> physics;
	std::shared_ptr<StageGraphics> graphics;

	std::shared_ptr<StagePainting> painter;
	std::shared_ptr<PaintBufferTexture> buffer;
	std::shared_ptr<PaintMarkTexture> markTexture;
	std::shared_ptr<StageAttributeSetter> attributeSetter;

	std::shared_ptr<WallGrahicsData> wallGraphics;

	
};

