#pragma once
#include "MtlReader.h"
#include "../Mesh.h"

bool objReader(std::string modelName, vector<Mesh>& mesh, ID3D11Device* device, ID3D11DeviceContext* immediateContext);