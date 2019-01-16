#pragma once
#include <memory>
class RigidData;
class PaintActorData;
class RigidData;
class InputManager;
class CharacterInput
{
public:
	CharacterInput();
	explicit CharacterInput(float rotationValue);
	virtual ~CharacterInput();
	virtual void stopInput()=0;
	virtual void update(const InputManager&)=0;

	void setRotationDelta(float rotationDelta);
	float getRotationDelta()const;
	void setActorData(const std::shared_ptr<PaintActorData>&);
	void setRIgidData(const std::shared_ptr<RigidData>& rigid);
	
protected:
	const std::shared_ptr<PaintActorData>& getActorData();
	const std::shared_ptr<RigidData>& getActorRigid();
private:
	std::shared_ptr<PaintActorData> data;
	std::shared_ptr<RigidData> rigid;
	float rotationValue;
};

