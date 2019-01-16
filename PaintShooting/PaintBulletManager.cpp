#include "PaintBulletManager.h"
#include "GraphicIBL.h"
#include "CharacterPhysics.h"
#include "StagePhysics.h"

#include "PaintColorSet.h"
#include "Particle3D.h"
#include "MaterialPBR.h"

#include "PaintBullet.h"

PaintBulletManager::PaintBulletManager(int maxBullets, std::shared_ptr<PaintColorSet>& color) :
	particle(Particle3D::createSpherePartecle(maxBullets)),
	team0Bullets(maxBullets),
	team1Bullets(maxBullets),
	colorset(color),
	material(std::make_unique<MaterialPBR>(Vector3f(0.0f,0.0f,0.0f),0.2f,0.2f,1.0f))
{

}

PaintBulletManager::~PaintBulletManager()
{
}

void PaintBulletManager::createBullet(const Vector3f & position, float scale, const Vector3f & velocity, int teamIndex,float paintScale,float damage)
{

	if (teamIndex == 0)
	{
		createBulletRoutain(team0Bullets, position, scale, velocity, paintScale,0,damage);
	}
	else if (teamIndex == 1)
	{
		createBulletRoutain(team1Bullets, position, scale, velocity, paintScale,1,damage);
	}
	else
	{
		return;
	}
}


void PaintBulletManager::update(float dt, float g)
{
	updateBullets(team0Bullets, dt, g);
	updateBullets(team1Bullets, dt, g);

}

void PaintBulletManager::collision(CharacterPhysics &character)
{
	collision(team0Bullets, character);
	collision(team1Bullets, character);

}

void PaintBulletManager::collision(StagePhysics &stage)
{
	collision(team0Bullets, stage);
	collision(team1Bullets, stage);

}

void PaintBulletManager::draw(const GraphicIBL & g)
{
	material->setAlbedo(colorset->getColor(0));
	draw(team0Bullets, g, *particle, *material);
	
	material->setAlbedo(colorset->getColor(1));
	draw(team1Bullets, g, *particle, *material);
}

void PaintBulletManager::createBulletRoutain(std::vector<PaintBullet>& bullets, const Vector3f& position, float scale, const Vector3f& velocity,float paintScale,int teamIndex,float damage)
{
	for (auto& bullet : bullets)
	{
		if (!bullet.isEntity)
		{
			bullet.isEntity = true;
			bullet.velocity = velocity;
			bullet.position = position;
			bullet.scale = scale;
			bullet.paintScale = paintScale;
			bullet.teamIndex = teamIndex;
			bullet.damage = damage;
			bullet.remain = 100.0;
			return;
		}
	}

	//std::cout << "bullet not remain" << std::endl;
}

void PaintBulletManager::updateBullets(std::vector<PaintBullet>& bullets,float dt,float g)
{
	for (auto& bullet : bullets)
	{
		if (bullet.isEntity)
		{
			bullet.velocity.y -= g * dt;
			bullet.position += bullet.velocity*dt;
			bullet.remain -= dt;

			if (bullet.remain < 0)
			{
				bullet.isEntity = false;
			}

		}
	}
}

void PaintBulletManager::collision(std::vector<PaintBullet>& bullets, CharacterPhysics & character)
{
	for (auto& bullet : bullets)
	{
		if (bullet.isEntity && bullet.damage != 0)
		{
			if (character.checkCollision(bullet))
			{
				bullet.isEntity = false;
			}
		}
	}
}

void PaintBulletManager::collision(std::vector<PaintBullet>& bullets, StagePhysics & stage)
{
	for (auto& bullet : bullets)
	{
		if (bullet.isEntity)
		{
			if (stage.collisionBullet(bullet))
			{
				bullet.isEntity = false;
			}
		}
	}
}

void PaintBulletManager::draw(std::vector<PaintBullet>& bullets, const GraphicIBL & g,Particle3D& particle,const MaterialPBR & material)
{
	int count = 0;
	for (const auto& bullet : bullets)
	{
		if (bullet.isEntity)
		{
			ParticleData data;
			data.size = bullet.scale;
			data.position = bullet.position;
			particle.setData(count, data);
			count++;
		}
	}
	
	g.draw(particle, count, material);
}
