#define _USE_MATH_DEFINES
#include "CharacterPhysics.h"
#include "RigidData.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "PaintActorData.h"
#include "CollisionSphere.h"
#include "StagePhysics.h"
#include "StageObstacle.h"
#include "MathAndPhysic.h"
#include "PaintBullet.h"
#include "Bomb.h"
CharacterPhysics::CharacterPhysics():
	maxVelocity(10.0f)
{

}


CharacterPhysics::~CharacterPhysics()
{
}

void CharacterPhysics::setMaxVelocityLength(float maxVelocity)
{
	this->maxVelocity = maxVelocity;
}

void CharacterPhysics::setRigid(const std::shared_ptr<RigidData>& rigid)
{
	this->rigid = rigid;
}

void CharacterPhysics::setActorData(const std::shared_ptr<PaintActorData>& data)
{
	this->data = data;
}


void CharacterPhysics::update(float dt, float gravity)
{
	if (data->getPrimalState() ==PaintActorPrimalState::ACTIVE)
	{
		//回転の計算
		Quaternion rotationY;
		rotationY.setRotation(Vector3f(0.0f, 1.0f, 0.0f), data->getAngleYaxis());
		Quaternion rotationX;
		rotationX.setRotation(Vector3f(1.0f, 0.0f, 0.0f), data->getAngleXaxis());
		//回転の更新
		Quaternion rotation = rotationY * rotationX;
		rigid->setRotation(rotation);
		//正面ベクトルの更新
		Vector3f face(0.0f, 0.0f, 1.0f);
		face = rotation.calcRotation(face);
		data->setFaceDirection(face);

		//速度と位置の計算と更新

		Vector3f position = rigid->getPosition();
		//正面方向ベクトルの計算
		Vector3f frontVeloctiy = face;
		frontVeloctiy.y = 0.0f;
		frontVeloctiy.normalize();
		//左右方向のベクトルの計算
		Quaternion sideRotation;
		sideRotation.setRotation(Vector3f(0.0f, 1.0f, 0.0f), -M_PI / 2.0f);
		Vector3f sideVelocity = sideRotation.calcRotation(frontVeloctiy);
		sideVelocity.normalize();
		//ベクトルの合成、速度の決定
		frontVeloctiy *= data->getFrontMove();
		sideVelocity *= data->getSideMove();

		Vector3f velocity = sideVelocity + frontVeloctiy;
		velocity *= maxVelocity;
		velocity.y = 0.0f;
		velocity.y = rigid->getVelocity().y;


		//位置の更新
		velocity.y -= gravity * dt;
		position += velocity * dt;


		rigid->setVelocity(velocity);
		rigid->setPosition(position);
	}

}

void CharacterPhysics::collision(const StagePhysics & stage)
{
	  
	Vector3f position = rigid->getPosition();
	Vector3f velocity = rigid->getVelocity();
	
	//jump
	float jumpVel = 5.0f;

	
	//障害物
	for (auto& obstacele : stage.getObstacles())
	{
		if (obstacele.checkCollision(position, rigid->getScale().x))
		{
			Vector3f modify = MyMath::Vector3AABBToPoint(obstacele.getPosition(), obstacele.getScale(),position);
			float d = rigid->getScale().x - modify.length();
			if (modify.y > 0)
			{
				if (velocity.y <= 0)
				{
					velocity.y = 0;
					if (data->getMotionState() == PaintActorMotionState::JUMPING)
					{
						velocity.y += jumpVel;
					}
				}
			}

			modify.normalize();
			modify *= d;
			position += modify;
		}
	}


	//壁判定
	if (position.x + rigid->getScale().x > stage.getPlaneWidth())
	{
		position.x = stage.getPlaneWidth() - rigid->getScale().x;
	}
	else if (position.x - rigid->getScale().x < -stage.getPlaneWidth())
	{
		position.x = -stage.getPlaneWidth() + rigid->getScale().x;
	}


	if (position.z + rigid->getScale().z > stage.getPlaneWidth())
	{
		position.z = stage.getPlaneWidth() - rigid->getScale().z;
	}
	else if (position.z - rigid->getScale().z < -stage.getPlaneWidth())
	{
		position.z = -stage.getPlaneWidth() + rigid->getScale().z;

	}


	//床判定
	if (position.y - rigid->getScale().y < stage.getHeight(position.x, position.z))
	{
		velocity.y = 0;
		position.y = rigid->getScale().y + stage.getHeight(position.x, position.z);
		if (data->getMotionState() == PaintActorMotionState::JUMPING)
		{
			velocity.y += jumpVel;
		}
	}





	rigid->setVelocity(velocity);
	rigid->setPosition(position);
}

void CharacterPhysics::collision(const Bomb & bomb)
{
	//爆風判定
	if (bomb.damageCollision(*rigid))
	{
		if (bomb.getTeamIndex() != data->getTeam())
		{
			std::cout << "damage" << std::endl;
			data->setLife(data->getLife() - bomb.getBombDamage());
		}
		
	}
}


bool CharacterPhysics::checkCollision(const PaintBullet &bullet)
{
	if (data->getPrimalState() == PaintActorPrimalState::INACTIVE)
	{
		return false;
	}

	CollisionSphere bulletSphere(bullet.position,bullet.scale);
	CollisionSphere characterSphere(rigid->getPosition(), rigid->getScale().x);

	if (bulletSphere.isCollision(characterSphere))
	{
		if (bullet.teamIndex != data->getTeam())
		{
			data->setLife(data->getLife() - bullet.damage);
		}
		
		return true;
	}
	else
	{
		return false;
	}


}

