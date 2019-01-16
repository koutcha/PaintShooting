#include "Stage.h"
#include "StageData.h"
#include "StagePhysics.h"
#include "StageGraphics.h"
#include "StagePainting.h"
#include "PaintBufferTexture.h"
#include "PaintMarkTexture.h"
#include "StageAttributeSetter.h"
#include "StageObstacle.h"

#include "Shape.h"
#include "SolidShapeIndex.h"
#include "PaintColorSet.h"



Stage::Stage(float stageScale, const std::shared_ptr<StagePhysics>& physics, const std::shared_ptr<StageGraphics>& graphics, const std::shared_ptr<PaintBufferTexture>& paintBuffer, const std::shared_ptr<PaintColorSet>& colorset):
data(std::make_shared<StageData>(stageScale, 30.0f, 9)),
physics(physics),
graphics(graphics),
buffer(paintBuffer),
markTexture(std::make_shared<PaintMarkTexture>("Resource\\Image\\ink.png", 2.0f))
{
	//テクスチャバッファの初期化
	paintBuffer->refresh();

	////障害物の設置
	Vector3f obstacleScale(4.0, 4.0, 4.0);
	float obstaclePos = 25.0;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
	
			float posX = -obstaclePos + obstaclePos * i;
			float posZ =-obstaclePos + obstaclePos * j;

			data->getObstales()[i * 3 + j].setScale(obstacleScale);
			data->getObstales()[i * 3 + j].setPosition(Vector3f(posX, obstacleScale.y, posZ));

		}
	}




	//障害物のモデルデータ(頂点情報)を読み込み
	std::shared_ptr<Shape> obstacleShape = SolidShapeIndex::createBoxShape();

	//レンダラーの追加アトリビュートの設定
	attributeSetter = std::make_shared<StageAttributeSetter>(buffer, colorset, 0.03f, 0.5f, 0.7f);
	//塗りシステム用のオブジェクトの作成
	painter = std::make_shared<StagePainting>(buffer, markTexture);

	////テクスチャに障害物情報(濡れない場所)を書き込む
	for (auto& obstacle : data->getObstales())
	{
		obstacle.setShape(obstacleShape);
		Vector3f position = obstacle.getPosition();
		buffer->renderNotPaintArea(position.x, position.z, obstacle.getScale(), *obstacle.getShape());
	}


	

	//内部オブジェクトに共有データを渡す
	physics->setStagePainting(painter);
	physics->setStageData(data);

	graphics->setStageData(data);
	graphics->setStagePainting(painter);
	graphics->setStageAttributeSetter(attributeSetter);



}

Stage::~Stage()
{
}
