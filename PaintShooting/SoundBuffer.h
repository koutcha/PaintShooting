#pragma once
#include "AL\al.h"
#include "AL\alc.h"
namespace Sound
{
	class SoundFile;
	class SoundBuffer
	{
	public:
		explicit SoundBuffer(const SoundFile& file);
		~SoundBuffer();
		ALuint getBufferID()const;
	private:
		ALuint bufferID;
	};

}
