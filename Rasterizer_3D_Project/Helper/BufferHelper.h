#pragma once
#include "BufferType.h"

#include <d3d11.h>
#include <vector>

bool SetupBuffers(ID3D11Device* device, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer, struct LightData& lightData, struct CamData& camData);
bool setupModelBuffers(ID3D11Device* device, ID3D11Buffer*& matrixBuffer, struct BufferData& matrixData, float width, float height);
bool setupModelBuffersDepth(ID3D11Device* device, ID3D11Buffer*& matrixBuffer, struct DepthBufferData& depthMatrixData, float width, float height);