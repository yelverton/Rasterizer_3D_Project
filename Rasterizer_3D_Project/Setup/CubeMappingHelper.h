#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>

bool SetupCubeMapping(ID3D11Device* device, UINT width, UINT height, ID3D11ShaderResourceView*& srvCubeMapping,
	ID3D11UnorderedAccessView* uavCubeMapping[], D3D11_VIEWPORT& viewPortCubeMapping);