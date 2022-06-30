#include "PipelineHelper.h"
#include "../Helper/ErrorLog.h"

#include <fstream>
#include <string>
#include <iostream>

bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderDepth,
	ID3D11PixelShader*& pShader, ID3D11ComputeShader*& cShader, std::string& vShaderByteCode, 
	std::string& vShaderByteCodeDepth)
{
	// VERTEX SHADER:

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

	// PIXEL SHADER:

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

	// COMPUTER SHADER:

	shaderData.clear();
	reader.close();

	reader.open("../x64/Debug/ComputeShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open()) {
		ErrorLog::Log("Could not open CS file!");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &cShader))) {
		ErrorLog::Log("Failed to create computer shader!");
		return false;
	}

	shaderData.clear();
	reader.close();

	// ([Depth] for shadow) Vertex Shader 
	reader.open("../x64/Debug/VertexShaderDepth.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		ErrorLog::Log("Could not open VS Depth file!");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShaderDepth)))
	{
		ErrorLog::Log("Failed to create vertex shader Depth!");
		return false;
	}

	vShaderByteCodeDepth = shaderData;

	shaderData.clear();
	reader.close();

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayoutVs, ID3D11InputLayout*& inputLayoutVsDepth,
	const std::string& vShaderByteCode, const std::string& vShaderByteCodeDepth)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(device->CreateInputLayout(inputDesc, _countof(inputDesc), vShaderByteCode.c_str(), 
		vShaderByteCode.length(), &inputLayoutVs)))
	{
		ErrorLog::Log("Failed to create Input Layout vs");
		return false;
	}

	if (FAILED(device->CreateInputLayout(inputDesc, _countof(inputDesc), vShaderByteCodeDepth.c_str(), 
		vShaderByteCodeDepth.length(), &inputLayoutVsDepth)))
	{
		ErrorLog::Log("Failed to create Input Layout vs Depth");
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
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&samplerDesc, &sampleState))) {
		ErrorLog::Log("Failed to create sampler state!");
		return false;
	}

	return true;
}

bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderDepth,
	ID3D11PixelShader*& pShader, ID3D11ComputeShader*& cShader, ID3D11InputLayout*& inputLayoutVS,
	ID3D11InputLayout*& inputLayoutVSDepth, ID3D11SamplerState*& sampleState)
{
	std::string vShaderByteCode, vShaderByteCodeDepth;

	if (!LoadShaders(device, vShader, vShaderDepth, pShader, cShader, vShaderByteCode, vShaderByteCodeDepth))
		return false;

	if (!CreateInputLayout(device, inputLayoutVS, inputLayoutVSDepth, vShaderByteCode, vShaderByteCodeDepth))
		return false;

	if (!CreateSampleState(device, sampleState))
		return false;

	return true;
}
