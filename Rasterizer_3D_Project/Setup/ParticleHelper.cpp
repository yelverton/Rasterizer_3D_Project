#include "ParticleHelper.h"
#include "../Helper/ErrorLog.h"

#include <string>
#include <iostream>

bool CreateUnorderedAccessView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11Buffer* particleBuffer, ID3D11UnorderedAccessView*& UAView, int particleSize)
{
	// get the address of the back buffer
	//ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&particleBuffer)))) {
		ErrorLog::Log("Failed to get back buffer for unorded access view!");
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 0;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = sizeof(particleBuffer) * 3;
	uavDesc.Buffer.Flags = 0;

	// use the back buffer address to create the uordered access view
	// null as description to base it on the backbuffers values
	if (FAILED(device->CreateUnorderedAccessView(particleBuffer, &uavDesc, &UAView))) {
		ErrorLog::Log("Failed to unoarded access view particle!");
		return false;
	}

	particleBuffer->Release();
	return true;
}

bool CreateParticleBuffer(ID3D11Device* device, ID3D11Buffer*& particleBuffer, struct ParticlePosition& particlePosition)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(ParticlePosition);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&particlePosition;
	data.SysMemPitch = data.SysMemPitch = 0;

	if (FAILED(device->CreateBuffer(&bufferDesc, &data, &particleBuffer)))
	{
		ErrorLog::Log("Failed to create particle Buffer");
		return false;
	}
}

bool SetupParticleHelper(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11UnorderedAccessView*& UAViewP,
	IDXGISwapChain*& swapChain, ID3D11Buffer*& particleBuffer, int particleSize, struct ParticlePosition& particlePosition)
{
	if (CreateParticleBuffer(device, particleBuffer, particlePosition))
		return false;

	if (CreateUnorderedAccessView(device, swapChain, particleBuffer, UAViewP, particleSize))
		return false;

	return true;
}
