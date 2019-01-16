#include "ActorCameraManager.h"
#include "PaintActor.h"
#include"Camera.h"





ActorCameraManager::ActorCameraManager(const std::shared_ptr<Camera>& camera, float distanceToTarget, float height):
	camera(camera),
	distance(distanceToTarget),
	height(height)
{
}

ActorCameraManager::~ActorCameraManager()
{
}

void ActorCameraManager::update(const PaintActor& target)
{
	Vector3f d = target.faceDirecion();
	d.y = 0;
	d.normalize();
	d *= -distance;
	d.y = height;

	d += target.getPosition();

	camera->setPosition(d);

	Vector3f pos = target.getPosition();
	pos.y = d.y;
	camera->setGaze(pos);
	
}
