#include "Mesh.h"
#include "../Helper/ErrorLog.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* immediateContext, std::vector<SimpleVertex> vertexTriangle,
	std::vector<DWORD> indexTriangle, std::vector<UINT> next, std::vector<UINT> size,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ambient,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> diffuse,
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> specular)
{
	this->device = device;
	this->immediateContext = immediateContext;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->next = next;
	this->size = size;

	if (FAILED(CreateVertexBuffer(vertexTriangle)))
		ErrorLog::Log("Failed to create Vertex Buffer!");

	if (FAILED(CreateIndexBuffer(indexTriangle)))
		ErrorLog::Log("Failed to create IndexBuffer!");

}

void Mesh::Draw()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < next.size(); i++)
	{
		immediateContext->PSSetShaderResources(0, 1, ambient[i].GetAddressOf());
		immediateContext->PSSetShaderResources(1, 1, diffuse[i].GetAddressOf());
		immediateContext->PSSetShaderResources(2, 1, specular[i].GetAddressOf());
		immediateContext->DrawIndexed(size[i], next[i], 0);
	}
}

void Mesh::DrawCubeCapping()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < next.size(); i++)
	{
		immediateContext->PSSetShaderResources(0, 1, ambient[i].GetAddressOf());
		immediateContext->DrawIndexed(size[i], next[i], 0);
	}
}

void Mesh::DrawPrePass()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < next.size(); i++)
	{
		immediateContext->DrawIndexed(size[i], next[i], 0);
	}
}


int Mesh::NrOfSubMashes()
{
	return next.size();
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
	this->size = mesh.size;
	this->next = mesh.next;
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



