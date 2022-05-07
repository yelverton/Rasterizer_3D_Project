#include "PipelineHelper.h"
#include "../Helper/ErrorLog.h"

#include <fstream>
#include <string>
#include <iostream>

bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../x64/Debug/VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		ErrorLog::Log("Could not open VS file!");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		ErrorLog::Log("Failed to create vertex shader!");
		return false;
	}

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	reader.open("../x64/Debug/PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		ErrorLog::Log("Could not open PS file!");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		ErrorLog::Log("Failed to create pixel shader!");
		return false;
	}

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, _countof(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create Input Layout!");
		return false;
	}

	return true;
}

bool CreateSampleState(ID3D11Device* device, ID3D11SamplerState*& sampleState)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT	hr = device->CreateSamplerState(&samplerDesc, &sampleState);
	if (FAILED(hr)) {
		ErrorLog::Log(hr, "Failed to create sampler state!");
		return false;
	}

	return true;
}

bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout, ID3D11SamplerState*& sampleState)
{
	std::string vShaderByteCode;

	if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
		return false;

	if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
		return false;

	if (!CreateSampleState(device, sampleState))
		return false;

	return true;
}
