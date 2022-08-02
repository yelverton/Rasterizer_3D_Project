#include "CubeMappingHelper.h"
#include "../Helper/ErrorLog.h"

bool CreateCubeMapping(ID3D11Device* device, UINT width, UINT height, ID3D11ShaderResourceView*& srvCubeMapping,
	ID3D11RenderTargetView* rtvCubeMapping[])
{
	int numberOfSides = 6;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texture2Dtemp;
	if (FAILED(device->CreateTexture2D(&desc, NULL, &texture2Dtemp)))
	{
		ErrorLog::Log("Failed to create texture2DArray");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = numberOfSides;

	if (FAILED(device->CreateShaderResourceView(texture2Dtemp, &srvDesc, &srvCubeMapping)))
	{
		ErrorLog::Log("Failed to setup srvCubeMapping!");
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 1;
	rtvDesc.Texture2DArray.MipSlice = 0;

	for (int i = 0; i < numberOfSides; i++)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = (UINT)i;
		if (FAILED(device->CreateRenderTargetView(texture2Dtemp, &rtvDesc, &rtvCubeMapping[i])))
		{
			ErrorLog::Log("Failed to setup rtvArray");
			return false;
		}
	}

	texture2Dtemp->Release();

	return true;
}

bool CreateDepthStencilCubeMapping(ID3D11Device* device, UINT width, UINT height, ID3D11DepthStencilView*& dsViewCubeMapping)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1; // kan behöva ändra till sex
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* dsTexture;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		ErrorLog::Log("Failed to create depth stencil texture!");
		return false;
	}

	if (FAILED(device->CreateDepthStencilView(dsTexture, 0, &dsViewCubeMapping)))
	{
		ErrorLog::Log("Failed to create depth stencil View!");
		return false;
	}

	dsTexture->Release();
	return true;
}

void SetViewportCubeMapping(D3D11_VIEWPORT& viewPortCubeMapping, UINT width, UINT height)
{
	viewPortCubeMapping.TopLeftX = 0;
	viewPortCubeMapping.TopLeftY = 0;
	viewPortCubeMapping.Width = static_cast<float>(width);
	viewPortCubeMapping.Height = static_cast<float>(height);
	viewPortCubeMapping.MinDepth = 0;
	viewPortCubeMapping.MaxDepth = 1;
}


bool SetupCubeMapping(ID3D11Device* device, UINT width, UINT height, ID3D11ShaderResourceView*& srvCubeMapping,
	ID3D11RenderTargetView* rtvCubeMapping[], ID3D11DepthStencilView*& dsViewCubeMapping, 
	D3D11_VIEWPORT& viewPortCubeMapping)
{
	if (!CreateCubeMapping(device, width, height, srvCubeMapping, rtvCubeMapping))
		return false;

	if (!CreateDepthStencilCubeMapping(device, width, height, dsViewCubeMapping))
		return false;

	SetViewportCubeMapping(viewPortCubeMapping, width, height);

	return true;
}