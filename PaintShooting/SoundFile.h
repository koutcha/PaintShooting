#pragma once
#include <memory>
#include "AL\al.h"
#include "AL\alc.h"
namespace Sound
{
	class SoundFile
	{
	public:
		explicit SoundFile(const char* filename);
		~SoundFile();

		void loadWav(const char* filename);
		bool setDataOnALBuffer(int bufferID)const;
		bool checkHasData()const;
	private:
		bool hasData;

		ALuint format;
		ALuint dataSize;
		size_t pcmFrec;
		std::unique_ptr<short[]> data16;
		std::unique_ptr<char[]> data8;
	};


}
