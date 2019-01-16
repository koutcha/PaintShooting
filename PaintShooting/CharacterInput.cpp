#define _USE_MATH_DEFINES
#include "CharacterInput.h"
#include "GL/glew.h"
#include"GLFW/glfw3.h"
#include "Window.h"
#include "RigidData.h"
#include "PaintActorData.h"
#include "InputManager.h"
#include "GamePadXInput.h"

CharacterInput::CharacterInput() :
	rotationValue(0.02f)
{
}

CharacterInput::CharacterInput(float rotationValue) :
	rotationValue(rotationValue)
{
}


CharacterInput::~CharacterInput()
{
}

void CharacterInput::stopInput()
{
	data->setAttackState(PaintActorAttackState::NEUTRAL);
	data->setMotionState(PaintActorMotionState::NEUTRAL);
	data->setFrontMove(0);
	data->setSideMove(0);
}


void CharacterInput::setRotationDelta(float rotationDelta)
{
	rotationValue = rotationDelta;
}

float CharacterInput::getRotationDelta() const
{
	return rotationValue;
}

void CharacterInput::setActorData(const std::shared_ptr<PaintActorData>& data)
{
	this->data = data;
}

void CharacterInput::setRIgidData(const std::shared_ptr<RigidData>& rigid)
{
	this->rigid = rigid;
}

const std::shared_ptr<PaintActorData>& CharacterInput::getActorData()
{
	return data;
}

const std::shared_ptr<RigidData>& CharacterInput::getActorRigid()
{
	return rigid;
}
