#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>

bool SetupShadowHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, D3D11_VIEWPORT& viewportShadow, UINT width, UINT height, 
	ID3D11DepthStencilView* dsViewShadow[], ID3D11ShaderResourceView* SRVShadow[], ID3D11RenderTargetView* gBufferRTV[],
	ID3D11ShaderResourceView* gBufferSRV[], IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView);