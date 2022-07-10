#pragma once
#include "../Helper/BufferType.h"

#include <Windows.h>
#include <d3d11.h>
#include <iostream>

bool SetupParticleHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11UnorderedAccessView*& UAViewP,
	IDXGISwapChain*& swapChain, ID3D11Buffer*& particleBuffer, int particleSize, struct ParticlePosition& particlePosition);