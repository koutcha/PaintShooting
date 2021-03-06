#pragma once
#include "CharacterInput.h"
#include "Vector3.h"
#include <memory>
#include <vector>
#include <random>
class GraphNode;
class PaintActor;
class CPUInput :
	public CharacterInput
{
public:
	CPUInput();
	explicit CPUInput(float stageScale);
	~CPUInput();
	// CharacterInput を介して継承されました
	virtual void stopInput() override;
	virtual void update(const InputManager&) override;
	//srcを内部メンバにコピーします
	void setMapNodes(const std::vector<GraphNode>& src);
	//監視対象の登録
	void addEnemy(const std::shared_ptr<const PaintActor>& paintActor);
private:
	enum class AIState
	{
		FOUND_ENEMY,
		PEACE,
		NONE,
	};

	void updatePeace(RigidData& rigid,PaintActorData& data);
	void updateFoundEnemy(RigidData& rigid, PaintActorData& data,const PaintActor& tragetEnemy);
	bool checkEnemyVisible(const PaintActor& actor,const Vector3f& myPosition,const Vector3f& myFaceDirection);

	AIState state;
	std::vector<GraphNode> nodes;
	std::vector<std::weak_ptr<const PaintActor>> enemyList;
	std::mt19937 engine;
	unsigned int nextNode;
	unsigned int serchNext();
	unsigned int serchNearest();

	float viewRadius;
	float viewAngle;

	
};

