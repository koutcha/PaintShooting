#include "SoundSource.h"
#include "SoundFile.h"
#include "SoundBuffer.h"
#include <iostream>
#include "AL\al.h"
#include "AL\alc.h"
using namespace Sound;


Sound::SoundSource::SoundSource():
	sourceID(0)
{
	alGenSources(1, &sourceID);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &sourceID);
}

void Sound::SoundSource::play(const SoundBuffer & buffer)const
{
	alSourceStop(sourceID);
	alSourcei(sourceID, AL_BUFFER, buffer.getBufferID());
	alSourcef(sourceID, AL_GAIN, 1.0);
	alSourcei(sourceID,AL_SOURCE_RELATIVE,AL_TRUE);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

	alSourcePlay(sourceID);
}



void Sound::SoundSource::play3D(const SoundBuffer & buffer, const Vector3f & position) const
{
	play3D(buffer, position, 1.0);
}

void Sound::SoundSource::play3D(const SoundBuffer & buffer, const Vector3f & position, float gain) const
{
	alSourceStop(sourceID);
	alSourcei(sourceID, AL_BUFFER, buffer.getBufferID());
	alSourcef(sourceID, AL_GAIN,gain);
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_FALSE);
	alSource3f(sourceID, AL_POSITION, position.x, position.y, position.z);

	alSourcePlay(sourceID);
}

void Sound::SoundSource::playRelative(const SoundBuffer & buffer, const Vector3f & relativePosition) const
{
	playRelative(buffer, relativePosition, 1.0);
}

void Sound::SoundSource::playRelative(const SoundBuffer & buffer, const Vector3f & relativePosition, float gain) const
{
	alSourceStop(sourceID);
	alSourcei(sourceID, AL_BUFFER, buffer.getBufferID());
	alSourcef(sourceID, AL_GAIN, 1.0);
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3f(sourceID, AL_POSITION, relativePosition.x, relativePosition.y, relativePosition.z);

	alSourcePlay(sourceID);
}

void Sound::SoundSource::playLoop(const SoundBuffer & buffer)
{
	alSourceStop(sourceID);
	alSourcei(sourceID, AL_BUFFER, buffer.getBufferID());
	alSourcef(sourceID, AL_GAIN, 1.0);
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

	alSourcePlay(sourceID);
}

void Sound::SoundSource::stopLoop(const SoundBuffer & buffer)
{
}
