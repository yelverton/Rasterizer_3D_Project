#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>

bool SetupMainRender(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11RenderTargetView* gBufferRTV[],
	ID3D11ShaderResourceView* gBufferSRV[], IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView, UINT width, UINT height);