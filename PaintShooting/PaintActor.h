#pragma once
#include <memory>
#include "Vector3.h"
class CharacterInput;
class CharacterPhysics;
class CharacterAction;
class CharacterGraphics;
class RigidData;
class PaintActorData;
class PaintBulletManager;

class PaintWeapon;
class Bomb;

namespace Sound
{
	class SoundPlayer;
}

struct PaintActorSetting;
class PaintActor
{
public:
	PaintActor(
		int playerID,
		int teamID,
		float iniLife,
		float respownTime,
		const Vector3f& iniPos,
		float iniAngleY,
		float maxVelocity,
		float angleDelta,
		const std::shared_ptr<CharacterInput>& input,
		const std::shared_ptr<CharacterPhysics>& physics,
		const std::shared_ptr<CharacterAction>& action,
		const std::shared_ptr<CharacterGraphics> graphics,
		const std::shared_ptr<PaintWeapon>& weapon,
		const std::shared_ptr<Bomb>& bomb,
		const std::shared_ptr<PaintBulletManager>& manager);

	PaintActor(
		const PaintActorSetting& setting,
		const std::shared_ptr<CharacterInput>& input,
		const std::shared_ptr<CharacterPhysics>& physics,
		const std::shared_ptr<CharacterAction>& action,
		const std::shared_ptr<CharacterGraphics> graphics,
		const std::shared_ptr<PaintWeapon>& weapon,
		const std::shared_ptr<Bomb>& bomb,
		const std::shared_ptr<PaintBulletManager>& manager);



	~PaintActor();
	const std::shared_ptr<CharacterInput>& getInput();
	const std::shared_ptr<CharacterPhysics>& getPhysics();
	const std::shared_ptr<CharacterAction>& getAction();
	const std::shared_ptr<CharacterGraphics>& getGraphics();

	const Vector3f getPosition()const;
	const Vector3f faceDirecion()const;
	float getLife()const;
private:

	//�ʒu�A���x�A��]�A�傫���f�[�^
	std::shared_ptr<RigidData> rigid;
	//���̑����(�����ʒu�A�����A���͏��)
	std::shared_ptr<PaintActorData> data;
	//�R���|�[�l���g
	std::shared_ptr<CharacterInput> input;
	std::shared_ptr<CharacterPhysics> physics;
	std::shared_ptr<CharacterGraphics> graphics;

	std::shared_ptr<CharacterAction> action;

};

