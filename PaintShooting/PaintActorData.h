#pragma once
#include "PaintActorState.h"
#include "Vector3.h"
class PaintActorData
{
public:
	//team index は　default値0
	PaintActorData(int playerID,float iniLife, float respownTime, const Vector3f& iniPos,float iniAngleX);
	~PaintActorData();
	void setPrimalState(PaintActorPrimalState state);
	void setAttackState(PaintActorAttackState state);
	void setMotionState(PaintActorMotionState state);
	void setAngleYaxis(float angle);
	void setAngleXaxis(float angle);

	void setLife(float life);
	void setTeam(int i);


	void setFrontMove(float frontVelocity);
	void setSideMove(float sideVelocity);
	void setFaceDirection(const Vector3f&);

	
	PaintActorPrimalState getPrimalState()const;
	PaintActorAttackState getAttackState()const;
	PaintActorMotionState getMotionState()const;
	float getAngleYaxis()const;
	float getAngleXaxis()const;
	float getLife()const;

	int getPlayerID()const;
	float getIniLife()const;
	float getRespownTime()const;
	const Vector3f getInipos()const;
	float getIniAngleYaxis()const;

	int getTeam()const;

	float getFrontMove()const;
	float getSideMove()const;

	const Vector3f getFaceDirection()const;
private:
	//playerID (0~1)
	int playerID;
	//初期ライフ
	float iniLife;
	//復活時間
	float respownTime;
	//初期位置
	Vector3f iniPos;
	//初期向きデータ
	float iniAngleY;
	//ステート
	//主要ステート(死んでるか生きてるか)
	PaintActorPrimalState  primeState;
	//攻撃ステート
	PaintActorAttackState actionState;
	//動きのステート　ジャンプするかしないかなど
	PaintActorMotionState motionState;
	//チーム(属する色)
	int teamIndex;
	//ライフ
	float life;
	//向きデータ
	float angleY;
	float angleX;
	//左右移動距離
	float frontMove;
	float sideMove;

	Vector3f faceDirection;
};

