#pragma once
#include "PaintActorState.h"
#include "Vector3.h"
class PaintActorData
{
public:
	//team index �́@default�l0
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
	//�������C�t
	float iniLife;
	//��������
	float respownTime;
	//�����ʒu
	Vector3f iniPos;
	//���������f�[�^
	float iniAngleY;
	//�X�e�[�g
	//��v�X�e�[�g(����ł邩�����Ă邩)
	PaintActorPrimalState  primeState;
	//�U���X�e�[�g
	PaintActorAttackState actionState;
	//�����̃X�e�[�g�@�W�����v���邩���Ȃ����Ȃ�
	PaintActorMotionState motionState;
	//�`�[��(������F)
	int teamIndex;
	//���C�t
	float life;
	//�����f�[�^
	float angleY;
	float angleX;
	//���E�ړ�����
	float frontMove;
	float sideMove;

	Vector3f faceDirection;
};

