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

bool SetupBuffers(ID3D11Device* device, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer,struct LightData& lightData, struct CamData& camData)
{
	if (!createLightBuffer(device, lightBuffer, lightData))
		return false;

	if (!createCamBuffer(device, camBuffer, camData))
		return false;

	return true;
}

bool setupModelBuffers(ID3D11Device* device, ID3D11Buffer*& matrixBuffer, struct BufferData& matrixData, float width, float height)
{
	XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&matrixData.world, XMMatrixTranspose(world));

	XMMATRIX View = XMMatrixLookAtLH({ 0,0,-2 }, { 0,0,-1 }, { 0,1,0 });
	XMStoreFloat4x4(&matrixData.view, XMMatrixTranspose(View));

	XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2, width / height, 0.1f, 100.0f);
	XMStoreFloat4x4(&matrixData.projection, XMMatrixTranspose(Projection));

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(BufferData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&matrixData;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	HRESULT hr = device->CreateBuffer(&desc, &data, &matrixBuffer);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create matrixBuffer");
		return false;
	}

	return true;
}

bool setupModelBuffersDepth(ID3D11Device* device, ID3D11Buffer*& matrixBuffer, struct DepthBufferData& depthMatrixData, float width, float height)
{
	XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&depthMatrixData.world, XMMatrixTranspose(world));

	XMMATRIX View = XMMatrixLookAtLH({ 0,0,-2 }, { 0,0,-1 }, { 0,1,0 });
	XMStoreFloat4x4(&depthMatrixData.view, XMMatrixTranspose(View));

	XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2, width / height, 0.1f, 100.0f);
	XMStoreFloat4x4(&depthMatrixData.projection, XMMatrixTranspose(Projection));

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(DepthBufferData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&depthMatrixData;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	HRESULT hr = device->CreateBuffer(&desc, &data, &matrixBuffer);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create matrixBuffer");
		return false;
	}

	return true;
}
