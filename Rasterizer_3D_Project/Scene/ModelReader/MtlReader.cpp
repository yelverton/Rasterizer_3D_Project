#include "MtlReader.h"

bool mtlReader(std::string mtlFile, std::string& ambient, std::string& diffuse, 
	std::string& specular, std::string readArea, std::vector<float>& specularExponent)
{
	std::ifstream file("Scene/models/" + mtlFile);
	if (!file.is_open())
	{
		//ErrorLog::Log("Failed to open " + mtlFile);
		return false;
	}

	string line, word;
	bool okayRead = false;

	istringstream iss;
	while (!file.eof())
	{
		getline(file, line);
		if (file.is_open())
		{
			iss.clear();
			iss.str(line);
			if (iss.good())
			{
				iss >> word;
				if (word == "newmtl")
				{
					iss >> word; 
					if (word == readArea)
						okayRead = true;
					else
						okayRead = false;
				}
				else if (okayRead)
				{
					if (word == "map_Kd") {
						iss >> word;
						ambient = word;
					}
					else if (word == "map_Ks") {
						iss >> word;
						diffuse = word;
					}
					else if (word == "map_Ka") {
						iss >> word;
						specular = word;
					}
					else if (word == "Ns")
					{
						iss >> word;
						specularExponent.push_back(stof(word));
					}
				}
			}

		}
	}

    return true;
}

