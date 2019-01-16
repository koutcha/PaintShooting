#include "ColorGenerator.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>

using json = nlohmann::json;
ColorGenerator::ColorGenerator(const std::string & colorsettingfile)
{
	std::ifstream reader(colorsettingfile, std::ios::in);
	if (!reader)
	{
		std::cout << colorsettingfile << "is not read" << std::endl;
		colorSets.resize(1);
		colorSets[0][0] = Vector3f(1.0f, 0.0f, 0.0f);
		colorSets[0][1] = Vector3f(0.0f, 1.0f, 0.0f);


	}
	else
	{
		json data;
		reader >> data;
		reader.close();

		colorSets.reserve(data["setsNumber"]);
		for (const auto& colorSetData : data["colorSet"])
		{
			std::array<Vector3f, 2> colorset;
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					colorset[i].setByIndex(j, colorSetData[i][j]);
				}
			}
			colorSets.emplace_back(colorset);
		}

	}


}

ColorGenerator::~ColorGenerator()
{
}

const std::array<Vector3f, 2> ColorGenerator::getRamdomColorSet() const
{
	std::random_device rnd;
	return colorSets[rnd() % colorSets.size()];
}
