#pragma once
#include "../Helper/BufferType.h"

#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <vector>

bool SetupParticleHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11UnorderedAccessView*& UAViewP,
	IDXGISwapChain*& swapChain, ID3D11Buffer*& particleBuffer, std::vector<XMFLOAT3>& particle, struct ParticlePosition& particlePosition,
	UINT width, UINT height, ID3D11DepthStencilView*& dsViewParticle, D3D11_VIEWPORT& viewPortParticle);