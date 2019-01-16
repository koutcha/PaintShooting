#include "SoundFile.h"
#include "inWave.h"

using namespace Sound;


Sound::SoundFile::SoundFile(const char * filename):
	hasData(false)
{
	loadWav(filename);
}

SoundFile::~SoundFile()
{
}

void SoundFile::loadWav(const char * filename)
{

	InWave wave(filename);
	if (!wave.isSuccessLoad())
	{
		std::cout << "error: can't load wav" << std::endl;
		hasData = false;
		return;
	}
	else
	{
		hasData = true;
	}

	unsigned int tempFormatID;
	unsigned int tempBitSize;

	pcmFrec = wave.sampleingRate();
	std::cout << "frec" << pcmFrec << std::endl;
	tempFormatID = wave.formatID();
	tempBitSize = wave.bitSize();


	long length = wave.length();


	if (tempBitSize == 8)
	{

		if (tempFormatID == 1)
		{
			dataSize = length * sizeof(char);
			data8 = std::move(std::make_unique<char[]>(length));
			wave.getData(data8.get());
			format = AL_FORMAT_MONO8;
		}
		else
		{

			dataSize = length * sizeof(char) * 2;
			std::unique_ptr<char[]> left(std::make_unique<char[]>(length));
			std::unique_ptr<char[]> right(std::make_unique<char[]>(length));
			wave.getData(left.get(), right.get());
			data8 = std::move(std::make_unique<char[]>(length * 2));
			for (int i = 0; i < wave.length(); ++i)
			{
				data8[2 * i] = left[i];
				data8[2 * i + 1] = right[i];
			}
			format = AL_FORMAT_STEREO8;
		}

	}
	else if (tempBitSize == 16)
	{
		if (tempFormatID == 1)
		{
			dataSize = length * sizeof(short);
			data16 =std::move(std::make_unique<short[]>(length));
			wave.getData(data16.get());
			format = AL_FORMAT_MONO16;
		}
		else
		{
			dataSize = length * sizeof(short) * 2;

			std::unique_ptr<short[]> left(std::make_unique<short[]>(length));
			std::unique_ptr<short[]> right(std::make_unique<short[]>(length));
			wave.getData(left.get(), right.get());
			data16 = std::move(std::make_unique<short[]>(length * 2));
			for (int i = 0; i < wave.length(); ++i)
			{
				data16[2 * i] = left[i];
				data16[2 * i + 1] = right[i];
			}
			format = AL_FORMAT_STEREO16;
		}

	}


}

bool Sound::SoundFile::setDataOnALBuffer(int bufferID) const
{

	if (hasData)
	{
		if (format == AL_FORMAT_STEREO16 || format == AL_FORMAT_MONO16)
		{
			alBufferData(bufferID, format, data16.get(), dataSize, pcmFrec);
			return true;
		}
		else
		{
			alBufferData(bufferID, format, data8.get(), dataSize, pcmFrec);
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool Sound::SoundFile::checkHasData() const
{
	return hasData;
}

