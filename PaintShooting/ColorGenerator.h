#pragma once
#include <array>
#include <vector>
#include <string>
#include "Vector3.h"
class ColorGenerator
{
public:
	ColorGenerator(const std::string& colorsettingfile);
	~ColorGenerator();
	const std::array<Vector3f, 2> getRamdomColorSet() const;
private:
	std::vector<std::array<Vector3f, 2>> colorSets;
};

