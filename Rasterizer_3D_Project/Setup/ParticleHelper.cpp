#include "ParticleHelper.h"
#include "../Helper/ErrorLog.h"

#include <string>
#include <iostream>

bool CreatePartices(std::vector<XMFLOAT3>& particle)
{
	for (int i = 0; i < 10; i++)
	{
		particle.push_back(XMFLOAT3(0, 1 + (i * 1.3f), 0));
	}

	return true;
}

bool CreateUnorderedAccessViewParticle(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11Buffer*& particleBuffer, ID3D11UnorderedAccessView*& UAViewP, std::vector<XMFLOAT3> particle)
{
	// get the address of the back buffer
	//ID3D11Texture2D* backBuffer = nullptr;
	//if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&particleBuffer)))) {
	//	ErrorLog::Log("Failed to get back buffer for unorded access view!");
	//	return false;
	//}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = particle.size() * 3;
	uavDesc.Buffer.Flags = 0;

	// use the back buffer address to create the uordered access view
	// null as description to base it on the backbuffers values
	if (FAILED(device->CreateUnorderedAccessView(particleBuffer, &uavDesc, &UAViewP))) {
		ErrorLog::Log("Failed to unoarded access view particle!");
		return false;
	}

	return true;
}

bool CreateParticleBuffer(ID3D11Device* device, ID3D11Buffer*& particleBuffer, struct ParticlePosition& particlePosition,
	std::vector<XMFLOAT3> particle)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * particle.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = particle.data();
	data.SysMemPitch = 0;

	if (FAILED(device->CreateBuffer(&bufferDesc, &data, &particleBuffer)))
	{
		ErrorLog::Log("Failed to create particle Buffer");
		return false;
	}
}

bool CreateDepthStencilParticle(ID3D11Device* device, UINT width, UINT height, ID3D11DepthStencilView*& dsViewParticle)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
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

	if (FAILED(device->CreateDepthStencilView(dsTexture, 0, &dsViewParticle)))
	{
		ErrorLog::Log("Failed to create depth stencil View!");
		return false;
	}

	dsTexture->Release();
	return true;
}

void SetViewportParticle(D3D11_VIEWPORT& viewPortParticle, UINT width, UINT height)
{
	viewPortParticle.TopLeftX = 0;
	viewPortParticle.TopLeftY = 0;
	viewPortParticle.Width = static_cast<float>(width);
	viewPortParticle.Height = static_cast<float>(height);
	viewPortParticle.MinDepth = 0;
	viewPortParticle.MaxDepth = 1;
}

bool SetupParticleHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11UnorderedAccessView*& UAViewP,
	IDXGISwapChain*& swapChain, ID3D11Buffer*& particleBuffer, std::vector<XMFLOAT3>& particle, struct ParticlePosition& particlePosition,
	UINT width, UINT height, ID3D11DepthStencilView*& dsViewParticle, D3D11_VIEWPORT& viewPortParticle)
{
	if (!CreatePartices(particle))
		return false;

	if (!CreateParticleBuffer(device, particleBuffer, particlePosition, particle))
		return false;

	if (!CreateUnorderedAccessViewParticle(device, swapChain, particleBuffer, UAViewP, particle))
		return false;

	if (!CreateDepthStencilParticle(device, width, height, dsViewParticle))
		return false;

	SetViewportParticle(viewPortParticle, width, height);

	return true;
}
