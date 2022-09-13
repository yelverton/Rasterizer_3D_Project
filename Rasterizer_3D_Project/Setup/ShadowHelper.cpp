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


void SetViewportShadow(D3D11_VIEWPORT& viewportShadow, UINT width, UINT height)
{
	viewportShadow.TopLeftX = 0;
	viewportShadow.TopLeftY = 0;
	viewportShadow.Width = static_cast<float>(width * 2);
	viewportShadow.Height = static_cast<float>(height * 2);
	viewportShadow.MinDepth = 0;
	viewportShadow.MaxDepth = 1;
}

bool SetupShadowHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, D3D11_VIEWPORT& viewportShadow, 
	UINT width, UINT height, ID3D11DepthStencilView* dsViewShadow[], ID3D11ShaderResourceView* SRVShadow[])
{
	// [Shadow Stage] Depth stencil
	if (!CreateDepthStencil(device, width, height, dsViewShadow, SRVShadow))
		return false;

	// [Shadow Stage] ViewPort:
	SetViewportShadow(viewportShadow, width, height);

    return true;
}
