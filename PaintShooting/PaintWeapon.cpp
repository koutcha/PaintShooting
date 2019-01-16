#include "PaintWeapon.h"
#include "PaintBulletManager.h"
#include "PaintActorData.h"
#include "RigidData.h"
#include "MathAndPhysic.h"
#include "SoundManager.h"
#include "nlohmann/json.hpp"
#include <fstream>

WeaponSettingData PaintWeapon::createSettingFromJson(const std::string & filepath)
{

	WeaponSettingData data;
	auto setDefault = [&]()
	{
		data.bulletScale = 0.2f;
		data.paintScale = 2.0f;
		data.velocity = 20.0f;
		data.shakeWidth = 2.0f;
		data.coolDownFrame = 3;
		data.damage = 10.0f;
	};
	std::ifstream reader(filepath);
	if (reader)
	{
		nlohmann::json setting;
		reader >> setting;
		
		data.bulletScale = setting["bulletscale"];
		data.paintScale = setting["paintscale"];
		data.velocity = setting["velocity"];
		data.shakeWidth = setting["shake"];
		data.coolDownFrame = setting["cooldown"];
		data.damage = setting["damage"];
	}
	else
	{
		std::cout << "can't read" << filepath << std::endl;
		setDefault();
	}
	
	
 
	return data;
}



PaintWeapon::PaintWeapon(const std::shared_ptr<PaintBulletManager>& bullets, const std::shared_ptr<SoundManager>& sounds, const WeaponSettingData& setting):
	manager(bullets),
	bulletSize(setting.bulletScale),
	paintSize(setting.paintScale),
	iniVelocity(setting.velocity),
	shakeWidth(setting.shakeWidth),
	coolDownFrame(setting.coolDownFrame),
	damage(setting.damage),
	sounds(sounds),
	count(0)
{
}

PaintWeapon::~PaintWeapon()
{
}

bool PaintWeapon::shoot(const PaintActorData &actorData,const RigidData& rigidData)
{

	//std::cout << "weapon attack" << std::endl;
	if (count > 0)
	{
		return false;
	}
	else
	{
		//std::cout << "fire" << std::endl;

		Vector3f position = rigidData.getPosition();
		Vector3f direction = actorData.getFaceDirection();
		position += direction;
		Vector3f yAxis(0.0, 1.0, 0.0);
		Vector3f bias = yAxis.cross(direction);
		bias.normalize();

		float  biasLength = MyMath::randamFloat(1.0f,0.0f, 1000) - 0.5f;

		bias *= biasLength * shakeWidth;
		manager->createBullet(position, bulletSize, direction*iniVelocity + bias, actorData.getTeam(), paintSize,damage);
		sounds->reservePlaying3D("shot", rigidData.getPosition());
		count = coolDownFrame;

		return true;
	}
}

void PaintWeapon::update()
{
	if (count > 0)
	{
		count--;
	}
}
