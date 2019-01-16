#pragma once
#include "Vector3.h"
struct PaintActorSetting
{
	//common 
	int playerID;
	int teamID;
	float iniLife;
	float respownTime;
	Vector3f iniPos;
	float iniAngle;
	//physics parameter
	float velocity;
	//input parameter
	int padID;
	float angleDelta;
};