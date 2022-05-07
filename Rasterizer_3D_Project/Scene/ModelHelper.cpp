#include "ModelHelper.h"
#include "../Helper/ErrorLog.h"
#include <fstream>
#include <iostream>

bool getModelInfo(std::vector<std::string>& fileName)
{
	std::ifstream myfile("Scene/ModelInfo.txt");
	if (!myfile.is_open())
	{
		ErrorLog::Log("Failed to open ModelInfo.txt!");
		return false;
	}
	else {
		std::string line;
		while (myfile >> line) {
			fileName.push_back(line);
		}
	}

	return true;
}

bool SetupModels(std::vector<std::string>& modelInfo)
{
	if (!getModelInfo(modelInfo))
		return false;

	return true;
}
