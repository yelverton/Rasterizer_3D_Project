#include "Mesh.h"
#include "../Helper/ErrorLog.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* immediateContext, std::vector<SimpleVertex> vertexTriangle,
	std::vector<DWORD> indexTriangle, std::vector<UINT> next, std::vector<UINT> size,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ambient,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> diffuse,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> specular,
	XMFLOAT3 world, int unique, XMVECTOR smallest, XMVECTOR biggest, std::vector<float> specularExponent)
{
	this->device = device;
	this->immediateContext = immediateContext;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->next = next;
	this->size = size;
	this->unique = unique;
	this->smallest = smallest;
	this->biggest = biggest;
	this->floatShiness = specularExponent;

	if (FAILED(CreateVertexBuffer(vertexTriangle)))
		ErrorLog::Log("Failed to create Vertex Buffer!");

	if (FAILED(CreateIndexBuffer(indexTriangle)))
		ErrorLog::Log("Failed to create IndexBuffer!");

	if (FAILED(SetupWorldMatrixs(world)))
		ErrorLog::Log("Failed to setup worldBuffer!");

	if (FAILED(SetupShinessBuffer()))
		ErrorLog::Log("Failed to setup specularBuffer!");

	CreateBoundingBox(world);
}

void Mesh::release()
{
	theWorldBuffer->Release();
	shinessBuffer->Release();
	
	ambient.clear();
	diffuse.clear();
	specular.clear();

	next.clear();
	size.clear();
}

void Mesh::Draw()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	SetContantBuffer();

	for (int i = 0; i < next.size(); i++)
	{
		immediateContext->PSSetShaderResources(0, 1, ambient[i].GetAddressOf());
		immediateContext->PSSetShaderResources(1, 1, diffuse[i].GetAddressOf());
		immediateContext->PSSetShaderResources(2, 1, specular[i].GetAddressOf());
		SetShinessBuffer(i);
		immediateContext->DrawIndexed(size[i], next[i], 0);
	}
}

void Mesh::DrawPrePass()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	SetContantBuffer();

	for (int i = 0; i < next.size(); i++)
	{
		immediateContext->DrawIndexed(size[i], next[i], 0);
	}
}

int Mesh::NrOfSubMashes()
{
	return next.size();
}

void Mesh::changeWorld(XMFLOAT3 world)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();
	Identity = XMMatrixTranslation(world.x, world.y, world.z);
	XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));
}

void Mesh::setUniqueId(int unique)
{
	this->unique = unique;
}

int Mesh::getUniqueId()
{
	return unique;
}

DirectX::BoundingBox Mesh::getBoundingBox()
{
	return boundingBox;
}


Mesh::Mesh(const Mesh& mesh)
{
	this->indexBuffer = mesh.indexBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->device = mesh.device;
	this->immediateContext = mesh.immediateContext;
	this->ambient = mesh.ambient;
	this->diffuse = mesh.diffuse;
	this->specular = mesh.specular;
	this->boundingBox = mesh.boundingBox;
	this->theWorldBuffer = mesh.theWorldBuffer;
	this->size = mesh.size;
	this->next = mesh.next;
	this->theWorld = mesh.theWorld;
	this->unique = mesh.unique;
	this->biggest = mesh.biggest;
	this->smallest = mesh.smallest;
	this->shineness = mesh.shineness;
	this->shinessBuffer = mesh.shinessBuffer;
	this->floatShiness = mesh.floatShiness;
}


HRESULT Mesh::CreateVertexBuffer(std::vector<SimpleVertex> vertexTriangle)
{
	if (vertexBuffer.Get() != nullptr)
		vertexBuffer.Reset();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * vertexTriangle.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexTriangle.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, vertexBuffer.GetAddressOf());
	return hr;
}

HRESULT Mesh::CreateIndexBuffer(std::vector<DWORD> indexTriangle)
{
	if (indexBuffer.Get() != nullptr)
		indexBuffer.Reset();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indexTriangle.data();
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer.GetAddressOf());
	return hr;
}

HRESULT Mesh::SetupWorldMatrixs(XMFLOAT3 world)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();
	Identity = XMMatrixTranslation(world.x, world.y, world.z);
	XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

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

	HRESULT hr = device->CreateBuffer(&desc, &data, &theWorldBuffer);
	return hr;
}

HRESULT Mesh::SetupShinessBuffer()
{
	shineness.padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	shineness.shiness = 1.0f;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(Shineness);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)&shineness;
	data.SysMemPitch = data.SysMemPitch = 0; // 1D resource 

	HRESULT hr = device->CreateBuffer(&desc, &data, &shinessBuffer);
	return hr;
}

void Mesh::CreateBoundingBox(XMFLOAT3 world)
{
	DirectX::BoundingBox::CreateFromPoints(boundingBox, smallest, biggest);
	DirectX::XMMATRIX tempScale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX tempRota = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX tempTrans = DirectX::XMMatrixTranslation(world.x, world.y, world.z);

	DirectX::XMMATRIX WMBB = XMMatrixIdentity();
	WMBB = DirectX::XMMatrixMultiply(tempScale, tempRota);
	WMBB = DirectX::XMMatrixMultiply(WMBB, tempTrans);

	boundingBox.Transform(boundingBox, WMBB);

	/*boundingBox.Transform(boundingBox, XMLoadFloat4x4(&theWorld.worldMatrix));*/

}

void Mesh::SetContantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	std::memcpy(subData.pData, &theWorld, sizeof(TheWorld));
	immediateContext->Unmap(theWorldBuffer, 0);
	immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);
}

void Mesh::SetShinessBuffer(int index)
{
	shineness.shiness = floatShiness[index];

	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(shinessBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	std::memcpy(subData.pData, &shineness, sizeof(Shineness));
	immediateContext->Unmap(shinessBuffer, 0);
	immediateContext->PSSetConstantBuffers(0, 1, &shinessBuffer);
}

void Mesh::setWorldPos(XMFLOAT3 world)
{
	if (FAILED(SetupWorldMatrixs(world)))
		ErrorLog::Log("Failed to setup worldBuffer!");

	CreateBoundingBox(world);
}


