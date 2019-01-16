#pragma once
#include <vector>
#include "Vector3.h"

namespace Sound
{
	class SoundSource;
	class SoundBuffer;
}
class SubManagerFor3DAudio
{
public:
	explicit SubManagerFor3DAudio(int sourceSize);
	~SubManagerFor3DAudio();
	void bindListner();

	void setPosition(const Vector3f& position);
	void setFace(const Vector3f& faceVector);
	void setUp(const Vector3f& up);
	
	const Vector3f& getPosition()const;
	const Vector3f& getFace()const;
	const Vector3f& getUp()const;

	void play(const Sound::SoundBuffer& buffer);
	//play 3D audio on World Coord
	void play3D(const Sound::SoundBuffer& buffer, const Vector3f& position);
	void play3D(const Sound::SoundBuffer& buffer, const Vector3f& position,float gain);

	//play 3D audio Relative Coord From Listener
	void playRelative(const Sound::SoundBuffer& buffer, const Vector3f& relativePosition);
	void playRelative(const Sound::SoundBuffer& buffer, const Vector3f& relativePosition,float gain);
	
private:
	Vector3f listenerPosition;
	Vector3f listenerUp;
	Vector3f listenerOrient;

	float gain;

	std::vector<Sound::SoundSource> sources;
	unsigned int nextUse;
};

