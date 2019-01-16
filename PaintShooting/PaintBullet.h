#include "Vector3.h"
struct PaintBullet
{
	PaintBullet() :
		isEntity(false)
	{

	};

	bool isEntity;
	//entityData
	Vector3f position;
	Vector3f velocity;
	float scale;
	//input Data
	float paintScale;
	int teamIndex;

	float damage;
	//
	float remain;
};

