#define _USE_MATH_DEFINES
#include "CPUInput.h"

#include <limits>
#include <algorithm>


#include "GLFW/glfw3.h"
#include "RigidData.h"
#include "PaintActorData.h"
#include "InputManager.h"
#include "GraphNode.h"
#include "PaintActor.h"

CPUInput::CPUInput()
{
}

CPUInput::CPUInput(float stageScale):
	nextNode(0),
	state(AIState::NONE),
	viewRadius(15.0f),
	viewAngle(M_PI*1.5f)
{
	float pos = stageScale - 10.0f;
	nodes.reserve(4);
	nodes.emplace_back(GraphNode(Vector3f(pos, 0, pos), { 1,2 }));
	nodes.emplace_back(GraphNode(Vector3f(-pos, 0, pos), { 0,3 }));
	nodes.emplace_back(GraphNode(Vector3f(pos, 0, -pos), { 0,3 }));
	nodes.emplace_back(GraphNode(Vector3f(-pos, 0, -pos), { 1,2 }));
	
	enemyList.reserve(5);

}


CPUInput::~CPUInput()
{
}

void CPUInput::update(const InputManager &inputManager)
{

	if (state ==AIState::NONE)
	{
		nextNode = serchNearest();
		
		state = AIState::PEACE;
	}

	auto& data = getActorData();
	auto& rigid = getActorRigid();

	//“ü—Íó‘Ô‚Ì‰Šú‰»
	data->setFrontMove(0);
	data->setSideMove(0);
	data->setAttackState(PaintActorAttackState::NEUTRAL);
	data->setMotionState(PaintActorMotionState::NEUTRAL);

	data->setAttackState(PaintActorAttackState::SHOOTING_PAINT_BULLET);

	Vector3f face = data->getFaceDirection();
	face.y = 0;


	state = AIState::PEACE;
	std::shared_ptr<const PaintActor> enemy;
	for (const auto& enemyPtr : enemyList)
	{
		if (enemy = enemyPtr.lock())
		{
			if (checkEnemyVisible(*enemy, rigid->getPosition(), face))
			{
				state = AIState::FOUND_ENEMY;
				break;
			}
			
		}
	}

	switch (state)
	{
	case AIState::PEACE:
		updatePeace(*rigid, *data);
		break;
	case AIState::FOUND_ENEMY:
		updateFoundEnemy(*rigid, *data, *enemy);
		break;
	case AIState::NONE:
		break;
	default:
		break;
	}
}

void CPUInput::updatePeace(RigidData& rigid, PaintActorData& data)
{
	Vector3f face = data.getFaceDirection();
	face.y = 0.0f;
	face.normalize();
	Vector3f target;

	//ƒJƒƒ‰‚Ì1ƒtƒŒ[ƒ€“–‚½‚è‚Ì‰ñ“]Šp“x
	float angleVary = getRotationDelta();
	//¶‰E yŽ²‰ñ“]
	float yAngle = data.getAngleYaxis();
	//ã‰º xŽ²‰ñ“]
	float xAngle = data.getAngleXaxis();


	if (nodes.size() > 0)
	{
		target = nodes[nextNode].getPosition() - rigid.getPosition();
		if (target.length() < 2.0f)
		{
			data.setFrontMove(0.0);
			nextNode = serchNext();

			target = nodes[nextNode].getPosition() - rigid.getPosition();
		}
		else
		{
			data.setFrontMove(1.0);
		}



		target.y = 0.0f;
		target.normalize();

		float angle = acos(face.dot(target));
		if (angle > 0.3f)
		{
			data.setFrontMove(0.0);
		}
		float angleSign = face.cross(target).y;

		if (angleSign > 0.0f)
		{
			yAngle += angleVary;
		}
		else if (angleSign < 0.0f)
		{
			yAngle -= angleVary;
		}
	}

	//’l‚ÌC³
	if (yAngle > 2.0f* M_PI)
	{
		yAngle = 0;
	}
	if (yAngle < 0)
	{
		yAngle = 2.0f * M_PI;
	}

	data.setAngleYaxis(yAngle);
}

void CPUInput::updateFoundEnemy(RigidData & rigid, PaintActorData & data, const PaintActor& targetEnemy)
{

	Vector3f face = data.getFaceDirection();
	face.y = 0.0f;
	face.normalize();
	Vector3f target = targetEnemy.getPosition() - rigid.getPosition();
	target.y = 0.0f;
	target.normalize();

	//ƒJƒƒ‰‚Ì1ƒtƒŒ[ƒ€“–‚½‚è‚Ì‰ñ“]Šp“x
	float angleVary = getRotationDelta();
	//¶‰E yŽ²‰ñ“]
	float yAngle = data.getAngleYaxis();
	//ã‰º xŽ²‰ñ“]
	float xAngle = data.getAngleXaxis();






	float angle = acos(face.dot(target));
	float angleSign = face.cross(target).y;

	if (angleSign > 0.0f)
	{
		yAngle += angleVary;
	}
	else if (angleSign < 0.0f)
	{
		yAngle -= angleVary;
	}
	

	//’l‚ÌC³
	if (yAngle > 2.0f* M_PI)
	{
		yAngle = 0;
	}
	if (yAngle < 0)
	{
		yAngle = 2.0f * M_PI;
	}

	data.setAngleYaxis(yAngle);
}


bool CPUInput::checkEnemyVisible(const PaintActor &enemy,const Vector3f& myPosition, const Vector3f& myFaceDirection)
{
	if (enemy.getLife() > 0.0f)
	{
		Vector3f enemyTarget = enemy.getPosition() - myPosition;
		enemyTarget.y = 0;

		float sqrtDist = enemyTarget.squaredLength();
		if (sqrtDist < viewRadius*viewRadius)
		{
			enemyTarget.normalize();
			float angle = acos(myFaceDirection.dot(enemyTarget));
			if (angle < viewAngle/2.0f)
			{
				return true;
			}
		}
	}

	return false;
}


unsigned int CPUInput::serchNext()
{

	const auto& adjacentList = nodes[nextNode].getAdjacentList();
	int size = adjacentList.size();
	if (size == 0)
	{
		return nextNode;
	}

	unsigned int next = adjacentList[rand()%size];

	

	if (next >= nodes.size())
	{
		return nextNode;
	}
	else
	{
		return next;
	}

}

unsigned int CPUInput::serchNearest()
{
	float nearestLength= std::numeric_limits<float>::max();
	int next = nextNode;

	for (size_t i = 0; i < nodes.size(); i++)
	{
		float length= (nodes[i].getPosition() - getActorRigid()->getPosition()).squaredLength();
		if (length < nearestLength)
		{
			nearestLength = length;
			next = i;
		}
	}

	return next;
}

void CPUInput::addEnemy(const std::shared_ptr<const PaintActor>& paintActor)
{
	enemyList.emplace_back(paintActor);
}

void CPUInput::stopInput()
{
	auto& data = getActorData();

	data->setAttackState(PaintActorAttackState::NEUTRAL);
	data->setMotionState(PaintActorMotionState::NEUTRAL);
	data->setFrontMove(0);
	data->setSideMove(0);
}

void CPUInput::setMapNodes(const std::vector<GraphNode>& src)
{
	//deepCopy
	this->nodes = src;
}
