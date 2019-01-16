#include "PaintActor.h"

#include "CharacterInput.h"
#include "CharacterGraphics.h"
#include "CharacterPhysics.h"
#include "CharacterAction.h"
#include "PaintActorSetting.h"

#include "RigidData.h"
#include "PaintActorData.h"
#include "PaintBulletManager.h"

#include "Quaternion.h"


PaintActor::PaintActor(
	int playerID,
	int teamID,
	float iniLife,
	float respownTime,
	const Vector3f& iniPos,
	float iniAngleY,
	float velocity,
	float angleDelta,
	const std::shared_ptr<CharacterInput>& input,
	const std::shared_ptr<CharacterPhysics>& physics,
	const std::shared_ptr<CharacterAction>& action,
	const std::shared_ptr<CharacterGraphics> graphics,
	const std::shared_ptr<PaintWeapon>& weapon,
	const std::shared_ptr<Bomb>& bomb,
	const std::shared_ptr<PaintBulletManager>& bulletManager):
	rigid(std::make_shared<RigidData>()),
	data(std::make_shared<PaintActorData>(playerID,iniLife,respownTime,iniPos,iniAngleY)),
	input(input),
	physics(physics),
	action(action),
	graphics(graphics)
{
	if (teamID < 2 && teamID>0)
	{
		data->setTeam(teamID);
	}
	

	rigid->setPosition(Vector3f(iniPos));
	rigid->setScale(Vector3f(0.5, 0.5, 0.5));
	Quaternion rotation;
	rotation.setRotation(Vector3f(0, 1, 0), 0);
	rigid->setRotation(rotation);

	
	input->setActorData(data);
	input->setRIgidData(rigid);

	input->setRotationDelta(angleDelta);

	physics->setRigid(rigid);
	physics->setActorData(data);
	physics->setMaxVelocityLength(velocity);

	graphics->setRigid(rigid);
	graphics->setActorData(data);

	action->setActorData(data);
	action->setRigidData(rigid);
	action->setPaintBulletManager(bulletManager);


	action->setPaintWeapon(weapon);
	action->setBomb(bomb);
}

PaintActor::PaintActor(const PaintActorSetting & setting, const std::shared_ptr<CharacterInput>& input,
	const std::shared_ptr<CharacterPhysics>& physics, const std::shared_ptr<CharacterAction>& action,
	const std::shared_ptr<CharacterGraphics> graphics, const std::shared_ptr<PaintWeapon>& weapon, 
	const std::shared_ptr<Bomb>& bomb, 
	const std::shared_ptr<PaintBulletManager>& manager):
	PaintActor(setting.playerID,setting.teamID,setting.iniLife,setting.respownTime,setting.iniPos,setting.iniAngle,setting.velocity,setting.angleDelta,input,physics,action,graphics,weapon,bomb,manager)
{

}

PaintActor::~PaintActor()
{
}

const std::shared_ptr<CharacterInput>& PaintActor::getInput()
{
	return input;
}

const std::shared_ptr<CharacterPhysics>& PaintActor::getPhysics()
{
	return physics;
}

const std::shared_ptr<CharacterAction>& PaintActor::getAction()
{
	return action;
}

const std::shared_ptr<CharacterGraphics>& PaintActor::getGraphics()
{
	return graphics;
}


const Vector3f PaintActor::getPosition() const
{
	return rigid->getPosition();
}

const Vector3f PaintActor::faceDirecion() const
{
	return data->getFaceDirection();
}

float PaintActor::getLife() const
{
	return data->getLife();
}
