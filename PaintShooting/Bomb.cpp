#include "Bomb.h"

#include "StagePhysics.h"
#include "StageObstacle.h"
#include "MathAndPhysic.h"
#include "GraphicIBL.h"
#include "MaterialPBR.h"
#include "Shape.h"
#include "Matrix.h"
#include "PaintBulletManager.h"
#include "RigidData.h"
#include "PaintActorData.h"
#include "CollisionSphere.h"
#include "PaintColorSet.h"
#include "SoundManager.h"

#include "nlohmann/json.hpp"
#include <fstream>


Bomb::Bomb(const BombSetting & setting, const std::shared_ptr<Shape>& shape,  const std::shared_ptr<PaintColorSet>& colorset,const std::shared_ptr<MaterialPBR>& material, const std::shared_ptr<PaintBulletManager>& bullets, const std::shared_ptr<SoundManager>& sound):
	shape(shape),
	colorset(colorset),
	bullets(bullets),
	sound(sound),
	rigid(std::make_unique<RigidData>()),
	material(std::make_unique<MaterialPBR>(*material)),
	isEntity(false),
	velocity(setting.velocity),
	paintScale(setting.fragmentPaintScale),
	damageScale(setting.damageScale),
	bombDamage(setting.bombDamage),
	burstFrames(setting.burstFrames),
	isExplode(false),
	fragmentBulletNum(setting.fragmentBulletNum),
	burstCount(0),
	damageFrameCount(0),
	teamIndex(0)
{
	rigid->setScale(Vector3f(setting.size, setting.size, setting.size));
}


Bomb::~Bomb()
{
}

bool Bomb::getIsEnitity() const
{
	return isEntity;
}

float Bomb::getBombDamage() const
{
	return bombDamage;
}

int Bomb::getTeamIndex() const
{
	return teamIndex;
}

void Bomb::pitch(const RigidData& actorRigid, const PaintActorData& actorData)
{
	if (!isEntity)
	{
		float upperCorrect = 0.2;
		teamIndex = actorData.getTeam();
		burstCount = burstFrames;
		Vector3f launchPos = actorRigid.getPosition();
		launchPos.y -= actorRigid.getScale().y;
		launchPos.y += rigid->getScale().y + 0.01;
		rigid->setPosition(launchPos);
		Vector3f direction = actorData.getFaceDirection();
		direction.y = 0;
		float angle = atan2(direction.x, direction.z);
		direction *= velocity;
		direction.y += upperCorrect;
		Quaternion q;
		q.setRotation(Vector3f(0.0f, 1.0f, 0.0f), angle);
		rigid->setRotation(q);
		rigid->setVelocity(direction);

		sound->reservePlaying3D("bombShot", rigid->getPosition());

		isEntity = true;

	
	}
}

void Bomb::pitch(const Vector3f & position, const Vector3f & velocity)
{
	if (!isEntity)
	{
		burstCount = burstFrames;
		rigid->setPosition(position);
		rigid->setVelocity(velocity);
		isEntity = true;
	}
}

void Bomb::update(float dt, float gravity)
{
	isExplode = false;

	if (isEntity)
	{
		Vector3f position = rigid->getPosition();
		Vector3f velocity = rigid->getVelocity();
		velocity.y -= gravity * dt;
		position += velocity * dt;
		rigid->setPosition(position);
		rigid->setVelocity(velocity);
		burstCount--;
		
		

		if (burstCount == 0)
		{
			//îöî≠
			for (int i = 0; i < fragmentBulletNum; i++)
			{
				Vector3f bulletVelocity = Vector3f(MyMath::randamFloat(damageScale, -damageScale, 100), MyMath::randamFloat(5.0f, 0.0f, 100), MyMath::randamFloat(damageScale, -damageScale, 100));
				bullets->createBullet(rigid->getPosition(), 0.2f, bulletVelocity, teamIndex,paintScale, 0.0f);
			}
			sound->reservePlaying3D("bombExplode" ,rigid->getPosition());
			isEntity = false;
			isExplode = true;
			

		}
	}

}

void Bomb::collision(const StagePhysics & stage)
{
	if (!isEntity)
	{
		return;
	}
	Vector3f position = rigid->getPosition();
	Vector3f velocity = rigid->getVelocity();
	float  bodySize = rigid->getScale().x;

	float elasticity = 0.8f;
	float decayFraction = 0.95f;
	float sinkCorrect = 0.02f;

	//Ç±Ç±Ç©ÇÁ

	//è·äQï®
	for (auto& obstacele : stage.getObstacles())
	{
		if (obstacele.checkCollision(position, bodySize))
		{
			Vector3f modify = MyMath::Vector3AABBToPoint(obstacele.getPosition(), obstacele.getScale(), position);
			//ì_Ç™ÇﬂÇËçûÇÒÇ≈ÇÈ
			if (modify.squaredLength() == 0.0f)
			{
				std::cout << "inn" << std::endl;
				Vector3f check = position - obstacele.getPosition();
				int maxIndex = 0;
				float max = 0.0f;
				for (int i = 0; i < 3; i++)
				{
					float absValue = abs(check.getByIndex(i));
					if (max <absValue)
					{
						max = absValue;
						maxIndex = i;
					}
				}
				velocity.setByIndex(maxIndex, -velocity.getByIndex(maxIndex)*elasticity);
				modify = Vector3f(0, 0, 0);
				float obstacleScale = obstacele.getScale().getByIndex(maxIndex);
				float myScale = rigid->getScale().getByIndex(maxIndex);
				modify.setByIndex(maxIndex, MyMath::sign(check.getByIndex(maxIndex))*(obstacleScale + myScale));
				modify += obstacele.getPosition();
				position.setByIndex(maxIndex, modify.getByIndex(maxIndex));

			}
			else
			{
				float d = bodySize - modify.length();

				modify.normalize();
				modify *= d;
				position += modify;
				for (int i = 0; i < 3; i++)
				{
					if (modify.getByIndex(i) != 0)
					{
						velocity.setByIndex(i, -velocity.getByIndex(i)*elasticity);
					}
				}

			}

		}
	}

	






	//ï«îªíË
	if (position.x + bodySize > stage.getPlaneWidth())
	{
		position.x = stage.getPlaneWidth() - bodySize;
		velocity.x = -velocity.x*elasticity;
	}
	else if (position.x - bodySize < -stage.getPlaneWidth())
	{
		position.x = -stage.getPlaneWidth() + bodySize;
		velocity.x = -velocity.x*elasticity;
	}


	if (position.z + bodySize> stage.getPlaneWidth())
	{
		position.z = stage.getPlaneWidth() - bodySize;
		velocity.z = -velocity.z*elasticity;
	}
	else if (position.z - bodySize < -stage.getPlaneWidth())
	{
		position.z = -stage.getPlaneWidth() + bodySize;

		velocity.z = -velocity.z*elasticity;

	}

	//è∞îªíË
	if (position.y - bodySize < stage.getHeight(position.x, position.z))
	{

		position.y = bodySize + stage.getHeight(position.x, position.z);
		//å∏ë¨
		velocity *= decayFraction;
		//êUìÆëŒçÙ
		if (sinkCorrect > velocity.y)
		{
			velocity.y = -velocity.y*elasticity;
		}

	}

	rigid->setPosition(position);
	rigid->setVelocity(velocity);

}

bool Bomb::damageCollision(const RigidData & paintActorRigid)const
{
	if (isExplode)
	{
		CollisionSphere painterSphere(paintActorRigid);
		CollisionSphere damageSphere(rigid->getPosition(), damageScale);
		if (painterSphere.isCollision(damageSphere))
		{
			return true;
		}
	}
	return false;
}

void Bomb::draw(const GraphicIBL & g)
{
	if (isEntity)
	{
		material->setAlbedo(colorset->getColor(teamIndex));
		Vector3f position = rigid->getPosition();
		Vector3f velocity = rigid->getVelocity();
		float  bodySize = rigid->getScale().x;

		Matrix m = Matrix::translate(position)*Matrix::rotate(rigid->getRotation())*Matrix::scale(bodySize, bodySize, bodySize);
		g.draw(*shape, m, *material);
	}

}

BombSetting Bomb::createSettingJson(const std::string & filename)
{
	BombSetting data;

	auto setDefault = [&]()
	{
		data.size = 0.3;
		data.fragmentPaintScale = 1.0;
		data.damageScale = 2.5;
		data.bombDamage = 100;
		data.burstFrames = 100;
		data.fragmentBulletNum = 20;
	};

	std::ifstream reader(filename);
	if (reader)
	{
		nlohmann::json setting;
		reader >> setting;

		try
		{
			data.size = setting["sizeScale"];
			data.velocity = setting["velocity"];
			data.fragmentPaintScale = setting["fragmentPaintScale"];
			data.damageScale = setting["damageScale"];
			data.bombDamage = setting["bombDamage"];
			data.burstFrames = setting["burstFrame"];
			data.fragmentBulletNum = setting["fragmentNum"];
		}
		catch (nlohmann::json::type_error &e)
		{
			std::cerr << e.what() << std::endl;
			setDefault();
		
		}

	}
	else
	{
		std::cout << "can't read" << filename << std::endl;

		setDefault();
	}
	return data;
}

