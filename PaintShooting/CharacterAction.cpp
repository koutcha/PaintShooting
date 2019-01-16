#include "CharacterAction.h"
#include "PaintActorData.h"
#include "PaintActorState.h"
#include "RigidData.h"
#include "PaintWeapon.h"
#include "Bomb.h"
#include "PaintBulletManager.h"
#include "MathAndPhysic.h"
#include "GameInfomation.h"
#include "SoundManager.h"

CharacterAction::CharacterAction(const std::shared_ptr<SoundManager>& sounds):
	sounds(sounds)
{
}


CharacterAction::~CharacterAction()
{
}

void CharacterAction::setRigidData(const std::shared_ptr<RigidData>& rigid)
{
	this->rigid = rigid;
}

void CharacterAction::setActorData(const std::shared_ptr<PaintActorData>& actorData)
{
	this->data = actorData;
}

void CharacterAction::setPaintWeapon(const std::shared_ptr<PaintWeapon>& weapon)
{
	this->weapon = weapon;
}

void CharacterAction::setBomb(const std::shared_ptr<Bomb>& bomb)
{
	this->bomb = bomb;
}

void CharacterAction::setPaintBulletManager(const std::shared_ptr<PaintBulletManager>& bulletManager)
{
	this->bulletManager = bulletManager;
}

void CharacterAction::setGameInfomation(const std::shared_ptr<GameInfomation>& infomation)
{
	this->infomation = infomation;
}



void CharacterAction::update(float dt)
{



	if (data->getPrimalState() == PaintActorPrimalState::ACTIVE)
	{
		if (data->getAttackState() == PaintActorAttackState::SHOOTING_PAINT_BOMB)
		{
			bomb->pitch(*this->rigid, *this->data);
		}
		else if (data->getAttackState() == PaintActorAttackState::SHOOTING_PAINT_BULLET)
		{
			weapon->shoot(*this->data, *this->rigid);
		}

		//defeated
		if (data->getLife() <=0)
		{
			data->setPrimalState(PaintActorPrimalState::INACTIVE);
			waitTime = data->getRespownTime();
			int deathBulletNum = 30;
			//やられた時のパーティクル生成
			for (int i = 0; i < deathBulletNum; i++)
			{
				float maxShake = 5.0f;
				float minShake = -5.0f;
				float notchNum = 100;
				Vector3f velocity = Vector3f(MyMath::randamFloat(maxShake,minShake,notchNum), MyMath::randamFloat(maxShake, minShake, notchNum), MyMath::randamFloat(maxShake, minShake, notchNum));
				bulletManager->createBullet(rigid->getPosition(), 0.2f, velocity, (data->getTeam() + 1) % 2, 2.0f, 0.0f);
			}

			sounds->reservePlaying3D("death", rigid->getPosition());
		}

	}
	else if (data->getPrimalState() == PaintActorPrimalState::INACTIVE)
	{
		waitTime -= dt;
		if (waitTime <= 0)
		{
			//respown
			data->setPrimalState(PaintActorPrimalState::ACTIVE);
			rigid->setPosition(data->getInipos());
			data->setLife(data->getIniLife());
			data->setAngleYaxis(data->getIniAngleYaxis());
		}
	}
	//一応監視オブジェクト無しでも動くように
	if (const auto& info = infomation.lock())
	{

		info->sendCurrentColorIndex(data->getPlayerID(), data->getTeam());
		info->sendtLifeData(data->getPlayerID(), data->getLife());
	}
}
