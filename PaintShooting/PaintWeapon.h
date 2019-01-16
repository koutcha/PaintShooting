#pragma once
#include <memory>
#include <string>
class PaintBulletManager;
class PaintActorData;
class RigidData;
class SoundManager;

struct WeaponSettingData
{
	float bulletScale;
	float paintScale;
	float velocity;
	float shakeWidth;
	int coolDownFrame;
	float damage;
};
class PaintWeapon
{
public:
	static WeaponSettingData createSettingFromJson(const std::string& filepath);
	PaintWeapon(const std::shared_ptr<PaintBulletManager>& bullets,const std::shared_ptr<SoundManager>& sounds,const WeaponSettingData& setting);

	~PaintWeapon();
	bool shoot(const PaintActorData& actorData,const RigidData& actorRididData);
	void update();
private:
	std::shared_ptr<SoundManager> sounds;
	std::shared_ptr<PaintBulletManager> manager;

	float bulletSize;
	float paintSize;
	float iniVelocity;
	float shakeWidth;

	float damage;

	int coolDownFrame;

	int count;
};

