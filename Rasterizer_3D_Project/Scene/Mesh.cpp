#include "Mesh.h"
#include "../Helper/ErrorLog.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

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

//bool Mesh::createTextures(std::string a, std::string b, std::string s)
//{
//	std::string folder = "Models/Texture/";
//
//	if (allResourcesExist(a, b, s))
//	{
//		CreateShaderResourView(folder + "missingTexture", this->ambient.GetAddressOf());
//		CreateShaderResourView(folder + "missingTexture", this->diffuse.GetAddressOf());
//		CreateShaderResourView(folder + "missingTexture", this->specular.GetAddressOf());
//	}
//	else
//	{
//		CreateShaderResourView(folder + a, this->ambient.GetAddressOf());
//		CreateShaderResourView(folder + b, this->diffuse.GetAddressOf());
//		CreateShaderResourView(folder + s, this->specular.GetAddressOf());
//	}
//
//	return true;
//}
//
//bool Mesh::allResourcesExist(std::string ambient, std::string diffuse, std::string specular)
//{
//	bool found = false;
//	if (ambient == "" || diffuse == "" || specular == "")
//	{
//		found = true;
//	}
//
//	return found;
//}
//
//bool Mesh::CreateShaderResourView(std::string mtlPictures, ID3D11ShaderResourceView** lighting)
//{
//	int width, height, channel;
//	unsigned char* picture = stbi_load(mtlPictures.c_str(), &width, &height, &channel, STBI_rgb_alpha);
//	if (picture == NULL)
//	{
//		ErrorLog::Log("error" + mtlPictures);
//	}
//
//	D3D11_TEXTURE2D_DESC textureDesc;
//	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
//
//	textureDesc.Width = (UINT)width;
//	textureDesc.Height = (UINT)height;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1;
//	textureDesc.MiscFlags = 0;
//	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.SampleDesc.Quality = 0;
//	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	textureDesc.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA subResource;
//	ZeroMemory(&subResource, sizeof(D3D11_SUBRESOURCE_DATA));
//
//	subResource.pSysMem = picture;
//	subResource.SysMemPitch = (UINT)width * 4;
//
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
//	HRESULT hr = device->CreateTexture2D(&textureDesc, &subResource, &texture);
//	if (FAILED(hr)) {
//		ErrorLog::Log(hr, "Failed to create texture2D");
//		return false;
//	}
//
//	hr = device->CreateShaderResourceView(texture.Get(), nullptr, lighting);
//	if (FAILED(hr)) {
//		ErrorLog::Log(hr, "Failed to create shaderResourceView");
//		return false;
//	}
//
//	stbi_image_free(picture);
//	return true;
//}




