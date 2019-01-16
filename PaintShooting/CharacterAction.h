#pragma once
#include <memory>
class PaintActorData;
class RigidData;
class CharacterPhysics;
class PaintWeapon;
class PaintBulletManager;
class Bomb;

class GameInfomation;
class SoundManager;
class CharacterAction
{
public:
	explicit CharacterAction(const std::shared_ptr<SoundManager>& sounds);
	~CharacterAction();

	void setRigidData(const std::shared_ptr<RigidData>& rigid);
	void setActorData(const std::shared_ptr<PaintActorData>& actorData);
	void setPaintWeapon(const std::shared_ptr<PaintWeapon>& weapon);
	void setBomb(const std::shared_ptr<Bomb>& bomb);
	void setPaintBulletManager(const std::shared_ptr<PaintBulletManager>& bulletManager);
	void setGameInfomation(const std::shared_ptr<GameInfomation>& infomation);
	void update(float dt);
private:

	std::shared_ptr<SoundManager> sounds;

	std::shared_ptr<PaintActorData> data;
	std::shared_ptr<RigidData> rigid;
	std::shared_ptr<PaintWeapon> weapon;
	std::shared_ptr<Bomb> bomb;
	std::shared_ptr<PaintBulletManager> bulletManager;
	std::weak_ptr<GameInfomation> infomation;

	float waitTime;
};

