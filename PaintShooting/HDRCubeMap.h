#pragma once
#include <string>
class HDRCubeMap
{
public:
	HDRCubeMap(const std::string& hdrImage);
	~HDRCubeMap();

	void bind(unsigned int unit)const;
	void unbind(unsigned int unit)const;
private:
	static const std::string hdrToCubePath;
	unsigned int cubeMap;
	static void renderCube();
	
};

