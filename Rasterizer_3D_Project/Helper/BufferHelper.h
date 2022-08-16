#pragma once
#include "BufferType.h"

#include <d3d11.h>
#include <vector>

bool SetupBuffers(ID3D11Device* device, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer,
	ID3D11Buffer*& theWorldBuffer, ID3D11Buffer*& getDirectionBuffer, ID3D11Buffer*& getDTTimeBuffer,
	struct LightData& lightData, struct CamData& camData, struct TheWorld& theWorld, struct GetDirection& getDirection,
	struct GetDtTime& getDTTime, ID3D11Buffer*& lightBufferTwo, ID3D11Buffer*& lightBufferThree, ID3D11Buffer*& lightBufferFour,
	struct LightDataTwo& lightDataTwo, struct LightDataThree& lightDataThree, struct LightDataFour& lightDataFour);