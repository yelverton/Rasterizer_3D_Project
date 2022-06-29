#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext,
	IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv, ID3D11UnorderedAccessView*& UAView,
	ID3D11DepthStencilView*& dsView, ID3D11DepthStencilView*& dsViewShadow, D3D11_VIEWPORT& viewport,
	ID3D11ShaderResourceView*& SRVShadow, ID3D11RenderTargetView* gBufferRTV[6], ID3D11ShaderResourceView* gBufferSRV[6]);