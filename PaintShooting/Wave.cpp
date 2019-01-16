#include "Wave.h"
#include "wave.h"

#include <iostream>


using namespace std;

namespace Sound
{

	Wave::Wave()
	{
	}


	Wave::~Wave()
	{
	}

	void Wave::printHeader() const
	{
		std::cout << "[[HEADER]]" << std::endl;
		std::cout << "" << std::endl;

		std::cout << "[RIFF WAVE]" << std::endl;
		std::cout << "Chunk ID          : " << (char)header.chunkID[0]
			<< (char)header.chunkID[1]
			<< (char)header.chunkID[2]
			<< (char)header.chunkID[3] << std::endl;
		std::cout << "Size - 8          : " << header.size8 << " Byte" << std::endl;
		std::cout << "Wave ID           : " << (char)header.waveID[0]
			<< (char)header.waveID[1]
			<< (char)header.waveID[2]
			<< (char)header.waveID[3] << std::endl;
		std::cout << "" << std::endl;

		std::cout << "[FMT CHUNK]" << std::endl;
		std::cout << "Chunk ID          : " << (char)header.fmt.chunkID[0]
			<< (char)header.fmt.chunkID[1]
			<< (char)header.fmt.chunkID[2]
			<< (char)header.fmt.chunkID[3] << std::endl;
		std::cout << "Chunk Size        : " << header.fmt.chunkSize << " Byte" << std::endl;
		std::cout << "Format ID         : " << header.fmt.formatID << std::endl;
		std::cout << "Channels          : " << header.fmt.channelNum << std::endl;
		std::cout << "Sampling rate     : " << header.fmt.samplingRate << " Hz" << std::endl;
		std::cout << "Byte per second   : " << header.fmt.bytePerSec << " B/s" << std::endl;
		std::cout << "Block size        : " << header.fmt.blockSize << " Byte" << std::endl;
		std::cout << "Bit depth         : " << header.fmt.bitDepth << " bit" << std::endl;
		std::cout << "" << std::endl;

		std::cout << "[DATA CHUNK]" << std::endl;
		std::cout << "Chunk ID          : " << (char)header.data.chunkID[0]
			<< (char)header.data.chunkID[1]
			<< (char)header.data.chunkID[2]
			<< (char)header.data.chunkID[3] << std::endl;
		std::cout << "Data size         : " << header.data.dataSize << " Byte" << std::endl;
		std::cout << "" << std::endl;

	}

	long Wave::length() const
	{
		return header.data.dataSize / header.fmt.blockSize;
	}

	double Wave::lenghtSecond() const
	{
		return (double)header.data.dataSize / header.fmt.bytePerSec;
	}

	unsigned int Wave::formatID() const
	{
		return header.fmt.formatID;
	}

	unsigned int Wave::sampleingRate() const
	{
		return header.fmt.samplingRate;
	}

	unsigned int Wave::bitSize() const
	{
		return header.fmt.bitDepth;
	}


}