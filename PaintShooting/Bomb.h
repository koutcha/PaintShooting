#pragma once
#include <memory>
#include "Vector3.h"
class PaintColorSet;
class MaterialPBR;
class PaintBulletManager;
class GraphicIBL;
class Shape;

class RigidData;
class PaintActorData;

class StagePhysics;

class CollisionSphere;
class SoundManager;

struct BombSetting
{
	float size;
	float velocity;
	float fragmentPaintScale;
	float damageScale;
	float bombDamage;
	int burstFrames;
	int fragmentBulletNum;
};
class Bomb
{
public:
	Bomb(const BombSetting& setting,const std::shared_ptr<Shape>& shape,const std::shared_ptr<PaintColorSet>& colorset,const std::shared_ptr<MaterialPBR>& material,const std::shared_ptr<PaintBulletManager>& bullets,const std::shared_ptr<SoundManager>& sound);
	~Bomb();
	bool getIsEnitity()const;
	float getBombDamage()const;
	int getTeamIndex()const;
	void pitch(const RigidData& actorRigid , const PaintActorData& actorData);
	void pitch(const Vector3f& position, const Vector3f& direction);
	void update(float dt,float gravity);
	void collision(const StagePhysics& stage);
	bool damageCollision(const RigidData& paintActorRigid)const;
	void draw(const GraphicIBL& g);
	static BombSetting createSettingJson(const std::string& filename);

private:
	bool isEntity;

	std::unique_ptr<RigidData> rigid;
	//sound
	std::shared_ptr<SoundManager> sound;
	//graphics
	std::shared_ptr<Shape> shape;
	std::shared_ptr<PaintColorSet> colorset;
	std::unique_ptr<MaterialPBR> material;
	std::shared_ptr<PaintBulletManager> bullets;

	//setting
	float bodySize;
	float velocity;
	float paintScale;
	float damageScale;
	float bombDamage;
	int burstFrames;
	int fragmentBulletNum;

	//variable
	//counter
	bool isExplode;
	int burstCount;
	int damageFrameCount;
	//temaIndexForColor
	int teamIndex;

};

