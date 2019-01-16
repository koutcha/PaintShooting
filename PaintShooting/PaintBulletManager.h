#pragma once
#include "Vector3.h"
#include <vector>
#include <memory>

class ParticleData;
class Particle3D;

class PaintColorSet;
class MaterialPBR;

class StagePhysics;
class CharacterPhysics;

class GraphicIBL;
struct PaintBullet;

class PaintBulletManager
{
public:
	PaintBulletManager(int maxBullets,std::shared_ptr<PaintColorSet>& color);
	~PaintBulletManager();
	void createBullet(const Vector3f& position,float scale,const Vector3f& velocity,int teamIndex,float paintScale,float damage);
	void update(float dt, float g);
	void collision(CharacterPhysics&);
	void collision(StagePhysics&);
	void draw(const GraphicIBL& g);
private:
	

	//サブルーチン
	static void createBulletRoutain(std::vector<PaintBullet>& bullets,const Vector3f& position,float scale,const Vector3f& velocity,float paintScale,int teamIndex,float damage);
	static void updateBullets(std::vector<PaintBullet>& bullets,float dt,float g);
	static void collision(std::vector<PaintBullet>& bullets, CharacterPhysics& character);
	static void collision(std::vector<PaintBullet>& bullets, StagePhysics& stage);
	static void draw(std::vector<PaintBullet>& bullets, const GraphicIBL& g, Particle3D& particle,const MaterialPBR& material);



	std::vector<PaintBullet> team0Bullets;
	std::vector<PaintBullet> team1Bullets;

	std::shared_ptr<Particle3D> particle;
	std::shared_ptr<PaintColorSet> colorset;
	std::unique_ptr<MaterialPBR> material;
};

