#include "SubManagerFor3DAudio.h"
#include "SoundSource.h"
#include "AL\al.h"
#include "AL\alc.h"

SubManagerFor3DAudio::SubManagerFor3DAudio(int sourceSize) :
	sources(sourceSize),
	nextUse(0),
	listenerPosition(0, 0, 0),
	listenerOrient(0, 0, -1),
	listenerUp(0,1,0)
{
	if (sources.size() == 0)
	{
		sources.resize(1);
	}
}


SubManagerFor3DAudio::~SubManagerFor3DAudio()
{
}

void SubManagerFor3DAudio::bindListner()
{
	ALfloat pos[] = { listenerPosition.x,listenerPosition.y,listenerPosition.z };
	alListener3f(AL_POSITION,listenerPosition.x,listenerPosition.y,listenerPosition.z);
	ALfloat orient[] =
	{
		listenerOrient.x,listenerOrient.y,listenerOrient.z,
		listenerUp.x,listenerUp.y,listenerUp.z
	};

	alListenerfv(AL_ORIENTATION, orient);
}

void SubManagerFor3DAudio::setPosition(const Vector3f & position)
{
	this->listenerPosition = position;
}

void SubManagerFor3DAudio::setFace(const Vector3f & faceVector)
{
	this->listenerOrient = faceVector;
}

void SubManagerFor3DAudio::setUp(const Vector3f & up)
{
	this->listenerUp = up;
}

const Vector3f & SubManagerFor3DAudio::getPosition() const
{
	// TODO: return ステートメントをここに挿入します
	return listenerPosition;
}

const Vector3f & SubManagerFor3DAudio::getFace() const
{
	// TODO: return ステートメントをここに挿入します
	return listenerOrient;
}

const Vector3f & SubManagerFor3DAudio::getUp() const
{
	// TODO: return ステートメントをここに挿入します
	return listenerUp;
}

void SubManagerFor3DAudio::play(const Sound::SoundBuffer & buffer) 
{
	sources[nextUse].play(buffer);
	nextUse++;
	if (nextUse >= sources.size())
	{
		nextUse = 0;
	}
}

void SubManagerFor3DAudio::play3D(const Sound::SoundBuffer & buffer, const Vector3f & position)
{
	play3D(buffer, position, 1.0);
}

void SubManagerFor3DAudio::play3D(const Sound::SoundBuffer & buffer, const Vector3f & position, float gain)
{
	sources[nextUse].play3D(buffer, position,gain);
	nextUse++;
	if (nextUse >= sources.size())
	{
		nextUse = 0;
	}

}

void SubManagerFor3DAudio::playRelative(const Sound::SoundBuffer & buffer, const Vector3f & relativePosition)
{
	playRelative(buffer, relativePosition, 1.0);
}

void SubManagerFor3DAudio::playRelative(const Sound::SoundBuffer & buffer, const Vector3f & relativePosition, float gain)
{
	sources[nextUse].playRelative(buffer, relativePosition,gain);
	nextUse++;
	if (nextUse >= sources.size())
	{
		nextUse = 0;
	}
}
