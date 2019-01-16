#pragma once
#include <memory>
class Camera;
class PaintActor;
class ActorCameraManager
{
public:
	ActorCameraManager(const std::shared_ptr<Camera>&,float distanceToTarget,float height);
	~ActorCameraManager();
	void update(const PaintActor& target);
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<PaintActor> target;
	
	float height;
	float distance;
};

