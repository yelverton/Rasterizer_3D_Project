#pragma once
#include "../Mesh.h"
#include "../../Helper/ErrorLog.h"
#include "../../Helper/Vertex.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

bool mtlReader(std::string mtlFile, ID3D11ShaderResourceView*& ambient, ID3D11ShaderResourceView*& diffuse,
	ID3D11ShaderResourceView*& specular, std::string readArea, float& shineness, ID3D11Device* device);