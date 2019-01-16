#include "SoundBuffer.h"
#include "SoundFile.h"
#include <iostream>


Sound::SoundBuffer::SoundBuffer(const SoundFile &file):
	bufferID(0)
{
	alGenBuffers(1, &bufferID);
	if (file.setDataOnALBuffer(bufferID))
	{

	}
	else
	{
		std::cout << "SoundBuffer::FileObject has no data"<< std::endl;
	}
}

Sound::SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &bufferID);
}

ALuint Sound::SoundBuffer::getBufferID() const
{
	return bufferID;
}
