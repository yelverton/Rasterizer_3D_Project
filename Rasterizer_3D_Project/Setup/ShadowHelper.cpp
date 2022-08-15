#include "ShadowHelper.h"
#include "../Helper/ErrorLog.h"

#include <string>
#include <iostream>

bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11DepthStencilView* dsViewShadow[],
	ID3D11ShaderResourceView* SRVShadow[])
{
	// Texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = (width * 2);
	textureDesc.Height = (height * 2);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// DSView
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Flags = 0;

	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderDesc.Texture2D.MostDetailedMip = 0;
	shaderDesc.Texture2D.MipLevels = 1;
	
	for (int i = 0; i < 4; i++)
	{
		ID3D11Texture2D* dsTexture;
		if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
		{
			ErrorLog::Log("Failed to create depth stencil texture!");
			return false;
		}

		if (FAILED(device->CreateDepthStencilView(dsTexture, &depthStencilViewDesc, &dsViewShadow[i])))
		{
			ErrorLog::Log("Failed to create depth stencil View for shadow!");
			return false;
		}


		if (FAILED(device->CreateShaderResourceView(dsTexture, &shaderDesc, &SRVShadow[i])))
		{
			ErrorLog::Log("Failed to create SRV!");
			return false;
		}
		dsTexture->Release();
	}

	return true;
}

bool CreateUnorderedAccessView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11UnorderedAccessView*& UAView)
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
		ErrorLog::Log("Failed to get back buffer for unorded access view!");
		return false;
	}

	// use the back buffer address to create the uordered access view
	// null as description to base it on the backbuffers values
	if (FAILED(device->CreateUnorderedAccessView(backBuffer, NULL, &UAView))) {
		ErrorLog::Log("Failed to unoarded access view!");
		return false;
	}

	backBuffer->Release();
	return true;
}

bool CreateGBuffers(ID3D11Device* device, UINT width, UINT height,
	ID3D11RenderTargetView* gBufferRTV[6], ID3D11ShaderResourceView* gBufferSRV[6])
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* gBuffTex[6];

	for (int i = 0; i < 6; i++)
	{
		if (FAILED(device->CreateTexture2D(&texDesc, NULL, &gBuffTex[i])))
		{
			ErrorLog::Log("Failed to setup TEXTURE2D for computer shader in CreateGBuffer");
			return false;
		}

		if (FAILED(device->CreateRenderTargetView(gBuffTex[i], NULL, &gBufferRTV[i])))
		{
			ErrorLog::Log("Failed to setup RenderTargetView for computer shader in CreateGBuffer");
			return false;
		}

		if (FAILED(device->CreateShaderResourceView(gBuffTex[i], NULL, &gBufferSRV[i])))
		{
			ErrorLog::Log("Failed to setup ShaderResourceView for computer shader in CreateGBuffer");
			return false;
		}

		gBuffTex[i]->Release();
	}

	return true;
}

void SetViewportShadow(D3D11_VIEWPORT& viewportShadow, UINT width, UINT height)
{
	viewportShadow.TopLeftX = 0;
	viewportShadow.TopLeftY = 0;
	viewportShadow.Width = static_cast<float>(width * 2);
	viewportShadow.Height = static_cast<float>(height * 2);
	viewportShadow.MinDepth = 0;
	viewportShadow.MaxDepth = 1;
}

bool SetupShadowHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, D3D11_VIEWPORT& viewportShadow, UINT width, UINT height,
	ID3D11DepthStencilView* dsViewShadow[], ID3D11ShaderResourceView* SRVShadow[], ID3D11RenderTargetView* gBufferRTV[6],
	ID3D11ShaderResourceView* gBufferSRV[6], IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView)
{
	// [Shadow Stage] Depth stencil
	if (!CreateDepthStencil(device, width, height, dsViewShadow, SRVShadow))
		return false;

	// [Computer shader] Unorderd access view:
	if (!CreateUnorderedAccessView(device, swapChain, UAView))
		return false;

	// [Computer shader] Gbuffers:
	if (!CreateGBuffers(device, width, height, gBufferRTV, gBufferSRV))
		return false;

	// [Shadow Stage] ViewPort:
	SetViewportShadow(viewportShadow, width, height);

    return true;
}
