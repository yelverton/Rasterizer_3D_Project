#pragma once
#include "../../Helper/ErrorLog.h"
#include "../../Helper/Vertex.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

bool mtlReader(std::string mtlFile, std::string& ambient, std::string& diffuse, 
	std::string& specular, std::string readArea, std::vector<float>& specularExponent);