#include "Mesh.h"
#include "../Helper/ErrorLog.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* immediateContext, std::vector<SimpleVertex> vertexTriangle, std::vector<DWORD> indexTriangle)
{
	this->device = device;
	this->immediateContext = immediateContext;

	HRESULT hr = CreateVertexBuffer(vertexTriangle);
	if (FAILED(hr))
		ErrorLog::Log(hr, "Failed to create Vertex Buffer!");

	hr = CreateIndexBuffer(indexTriangle);
	if (FAILED(hr))
		ErrorLog::Log(hr, "Failed to create IndexBuffer!");
}

void Mesh::Draw()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	immediateContext->DrawIndexed(size, 0, 0);
}

Mesh::Mesh(const Mesh& mesh)
{
	this->indexBuffer = mesh.indexBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->device = mesh.device;
	this->immediateContext = mesh.immediateContext;
	this->size = mesh.size;
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

	size = indexTriangle.size();

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indexTriangle.data();
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer.GetAddressOf());
	return hr;
}
