#pragma once
#include "BufferType.h"

#include <d3d11.h>
#include <vector>

bool SetupBuffers(ID3D11Device* device, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer,
	ID3D11Buffer*& theWorldBuffer, ID3D11Buffer*& getDirectionBuffer, struct LightData& lightData, struct CamData& camData,
	struct TheWorld& theWorld, struct GetDirection &getDirection);