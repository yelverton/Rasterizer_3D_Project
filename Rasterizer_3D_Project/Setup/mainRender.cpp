#include "mainRender.h"
#include "../Helper/ErrorLog.h"

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

bool SetupMainRender(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11RenderTargetView* gBufferRTV[],
	ID3D11ShaderResourceView* gBufferSRV[], IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView, UINT width, UINT height)
{
	// [Computer shader] Unorderd access view:
	if (!CreateUnorderedAccessView(device, swapChain, UAView))
		return false;

	// [Computer shader] Gbuffers:
	if (!CreateGBuffers(device, width, height, gBufferRTV, gBufferSRV))
		return false;

	return true;
}
