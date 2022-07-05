#include "ModelHelper.h"
#include "../Helper/ErrorLog.h"
#include <fstream>
#include <iostream>

bool getModelInfo(std::vector<std::string>& fileName, std::vector<XMFLOAT3>& world)
{
	std::vector<std::string> worldPos;

	std::ifstream myfile("Scene/ModelInfo.txt");
	if (!myfile.is_open())
	{
		ErrorLog::Log("Failed to open ModelInfo.txt!");
		return false;
	}
	else {
		std::string line;
		std::string temp;
		for (int i = 0; i < 2; i++) {
			myfile >> line;
			fileName.push_back(line);
			worldPos.push_back("");
			myfile >> line;
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != '|')
					worldPos[worldPos.size() - 1] += line[i];
				else
					worldPos.push_back("");
			}
			world.push_back(XMFLOAT3{ stof(worldPos[0]), stof(worldPos[1]), stof(worldPos[2]) });
			worldPos.clear();
		}
	}

	return true;
}

bool SetupModels(std::vector<std::string>& modelInfo, std::vector<XMFLOAT3>& world)
{
	if (!getModelInfo(modelInfo, world))
		return false;

	return true;
}
