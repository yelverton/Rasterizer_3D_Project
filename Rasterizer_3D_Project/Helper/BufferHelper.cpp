#include "BufferHelper.h"
#include "ErrorLog.h"

bool createLightBuffer(ID3D11Device* device, ID3D11Buffer*& lightBuffer, struct LightData& lightData)
{
	// create light constant buffer
	lightData.lightPosition = XMFLOAT3(0.0f, 10.0f, -10.0f);
	lightData.shininess = 128.0f;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(LightData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&lightData;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	HRESULT hr = device->CreateBuffer(&desc, &data, &lightBuffer);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create lightBuffer");
		return false;
	}

	return true;
}

bool createCamBuffer(ID3D11Device* device, ID3D11Buffer*& camBuffer, struct CamData& camData)
{
	camData.cameraPosition = XMFLOAT3(0.0f, 0.0f, -2.0f);
	camData.padding = 0.0f;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(CamData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&camData;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	HRESULT hr = device->CreateBuffer(&desc, &data, &camBuffer);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create camBuffer");
		return false;
	}

	return true;
}

bool setupWorldMatrixs(ID3D11Device* device, ID3D11Buffer*& theWorldBuffer, struct TheWorld& theWorld)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(TheWorld);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&theWorld;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	if (FAILED(device->CreateBuffer(&desc, &data, &theWorldBuffer)))
	{
		ErrorLog::Log("Failed to create world Buffer");
		return false;
	}

	return true;
}

bool setupForwardVector(ID3D11Device* device, ID3D11Buffer*& getDirectionBuffer, struct GetDirection& getDirection)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(GetDirection);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&getDirection;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	if (FAILED(device->CreateBuffer(&desc, &data, &getDirectionBuffer)))
	{
		ErrorLog::Log("Failed to create world Buffer");
		return false;
	}

	return true;
}

bool SetupBuffers(ID3D11Device* device, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer,
	ID3D11Buffer*& theWorldBuffer, ID3D11Buffer*& getDirectionBuffer, struct LightData& lightData, struct CamData& camData,
	struct TheWorld& theWorld, struct GetDirection& getDirection)
{
	if (!createLightBuffer(device, lightBuffer, lightData))
		return false;

	if (!createCamBuffer(device, camBuffer, camData))
		return false;

	if (!setupWorldMatrixs(device, theWorldBuffer, theWorld))
		return false;
	
	if (!setupForwardVector(device, getDirectionBuffer, getDirection))
		return false;

	return true;
}
