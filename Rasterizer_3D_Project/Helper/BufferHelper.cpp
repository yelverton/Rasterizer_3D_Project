#include "BufferHelper.h"
#include "ErrorLog.h"

bool createLightBuffer(ID3D11Device* device, ID3D11Buffer*& lightBuffer, struct LightData& lightData)
{
	// create light constant buffer
	lightData.lightDirection = XMFLOAT3(-0.0f, -0.5f, -0.5f);
	lightData.padding = 0.0f;

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

bool createLightBufferTwo(ID3D11Device* device, ID3D11Buffer*& lightBuffer, struct LightDataTwo& lightData)
{
	// create light constant buffer
	lightData.posTwo = XMFLOAT3(5.0f, 5.0f, 0.0f);
	lightData.ranageTwo = 20.0f;
	lightData.dirTwo = XMFLOAT3(0.0f, -1.0f, 0.0f);
	lightData.coneTwo = 10.0f;
	lightData.cSpotTwo = XMFLOAT3(1.0f, 1.0f, 1.0f);
	lightData.paddingTwo = 0.0f;
	lightData.attTwo = XMFLOAT3(0.7f, 0.2f, 0.0f);	
	lightData.paddingTwoTwo = 0.0f;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(LightDataTwo);
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

bool createLightBufferThree(ID3D11Device* device, ID3D11Buffer*& lightBuffer, struct LightDataThree& lightData)
{
	// create light constant buffer
	lightData.posThree = XMFLOAT3(0.0f, 0.0f, 0.0f);
	lightData.ranageThree = 5.0f;
	lightData.dirThree = XMFLOAT3(0.0f, -1.0f, 0.0f);
	lightData.coneThree = 5.0f;
	lightData.cSpotThree = XMFLOAT3(1.0f, 1.0f, 1.0f);
	lightData.paddingThree = 0.0f;
	lightData.attThree = XMFLOAT3(0.7f, 0.2f, 0.0f);
	lightData.paddingThreeThree = 0.0f;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(LightDataThree);
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

bool createLightBufferFour(ID3D11Device* device, ID3D11Buffer*& lightBuffer, struct LightDataFour& lightData)
{
	// create light constant buffer
	lightData.posFour = XMFLOAT3(0.0f, 0.0f, 0.0f);
	lightData.ranageFour = 5.0f;
	lightData.dirFour = XMFLOAT3(0.0f, -1.0f, 0.0f);
	lightData.coneFour = 5.0f;
	lightData.cSpotFour = XMFLOAT3(1.0f, 1.0f, 1.0f);
	lightData.paddingFour = 0.0f;
	lightData.attFour = XMFLOAT3(0.7f, 0.2f, 0.0f);
	lightData.paddingFourFour = 0.0f;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(LightDataFour);
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

bool setupGetDtTime(ID3D11Device* device, ID3D11Buffer*& getDTTimeBuffer, struct GetDtTime& getDTTime)
{
	getDTTime.dt = 0.0f;
	getDTTime.paddingDt = XMFLOAT3(0.0f, 0.0f, 0.0f);

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(GetDirection);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&getDTTime;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	if (FAILED(device->CreateBuffer(&desc, &data, &getDTTimeBuffer)))
	{
		ErrorLog::Log("Failed to create world Buffer");
		return false;
	}

	return true;
}

bool SetupBuffers(ID3D11Device* device, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer,
	ID3D11Buffer*& theWorldBuffer, ID3D11Buffer*& getDirectionBuffer, ID3D11Buffer*& getDTTimeBuffer,
	struct LightData& lightData, struct CamData& camData, struct TheWorld& theWorld, struct GetDirection& getDirection,
	struct GetDtTime& getDTTime, ID3D11Buffer*& lightBufferTwo, ID3D11Buffer*& lightBufferThree, ID3D11Buffer*& lightBufferFour,
	struct LightDataTwo& lightDataTwo, struct LightDataThree& lightDataThree, struct LightDataFour& lightDataFour)
{
	if (!createLightBuffer(device, lightBuffer, lightData))
		return false;

	if (!createLightBufferTwo(device, lightBufferTwo, lightDataTwo))
		return false;

	if (!createLightBufferThree(device, lightBufferThree, lightDataThree))
		return false;

	if (!createLightBufferFour(device, lightBufferFour, lightDataFour))
		return false;

	if (!createCamBuffer(device, camBuffer, camData))
		return false;

	if (!setupWorldMatrixs(device, theWorldBuffer, theWorld))
		return false;
	
	if (!setupForwardVector(device, getDirectionBuffer, getDirection))
		return false;
	
	if (!setupGetDtTime(device, getDTTimeBuffer, getDTTime))
		return false;

	return true;
}
