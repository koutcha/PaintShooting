#include "PaintActorData.h"





PaintActorData::PaintActorData(int playerID,float iniLife, float respownTime, const Vector3f & iniPos,float iniAngleY):
	angleY(iniAngleY),
	angleX(0.0f),
	frontMove(0.0f),
	sideMove(0.0),
	teamIndex(0),
	primeState(PaintActorPrimalState::ACTIVE),
	iniPos(iniPos),
	iniLife(iniLife),
	life(iniLife),
	respownTime(respownTime),
	playerID(playerID),
	iniAngleY(iniAngleY)
{

}

PaintActorData::~PaintActorData()
{
}

void PaintActorData::setPrimalState(PaintActorPrimalState state)
{
	primeState = state;
}

void PaintActorData::setAttackState(PaintActorAttackState state)
{
	actionState = state;
}

void PaintActorData::setMotionState(PaintActorMotionState state)
{
	motionState = state;
}

void PaintActorData::setAngleYaxis(float angle)
{
	angleY = angle;
}

void PaintActorData::setAngleXaxis(float angle)
{
	angleX = angle;
}


void PaintActorData::setLife(float life)
{
	if (life < 0)
	{
		life = 0;
	}
	this->life = life;
}

void PaintActorData::setTeam(int i)
{
	teamIndex = i;
}

void PaintActorData::setFrontMove(float frontVelocity)
{
	frontMove = frontVelocity;
}

void PaintActorData::setSideMove(float sideVelocity)
{
	sideMove = sideVelocity;
}

void PaintActorData::setFaceDirection(const Vector3f & faceDirection)
{
	this->faceDirection = faceDirection;
}

PaintActorPrimalState PaintActorData::getPrimalState() const
{
	return primeState;
}

PaintActorAttackState PaintActorData::getAttackState() const
{
	return actionState;
}

PaintActorMotionState PaintActorData::getMotionState() const
{
	return motionState;
}

float PaintActorData::getAngleYaxis() const
{
	return angleY;
}

float PaintActorData::getAngleXaxis() const
{
	return angleX;
}

float PaintActorData::getLife() const
{
	return life;
}

int PaintActorData::getPlayerID() const
{
	return playerID;
}

float PaintActorData::getIniLife() const
{
	return iniLife;
}

float PaintActorData::getRespownTime() const
{
	return respownTime;
}

const Vector3f PaintActorData::getInipos() const
{
	return iniPos;
}

float PaintActorData::getIniAngleYaxis() const
{
	return iniAngleY;
}

int PaintActorData::getTeam() const
{
	return teamIndex;
}

float PaintActorData::getFrontMove() const
{
	return frontMove;
}

float PaintActorData::getSideMove() const
{
	return sideMove;
}

const Vector3f PaintActorData::getFaceDirection() const
{
	return faceDirection;
}
