#include "PipelineHelper.h"
#include "../Helper/ErrorLog.h"

#include <fstream>
#include <string>
#include <iostream>


bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderDepth, ID3D11VertexShader*& vShaderParticle,
	ID3D11GeometryShader*& gShaderParticle, ID3D11PixelShader*& pShader, ID3D11PixelShader*& pShaderParticle, ID3D11ComputeShader*& cShader, 
	ID3D11ComputeShader*& cShaderParticle, std::string& vShaderByteCode, std::string& vShaderByteCodeDepth, std::string& vShaderByteCodeParticle)
{
	std::string shaderData;
	std::ifstream reader;

	// VERTEX SHADER:
	
		// [PARTICLE]
	
		reader.open("../x64/Debug/VertexShaderParticle.cso", std::ios::binary | std::ios::ate);
		if (!reader.is_open())
		{
			ErrorLog::Log("Could not open VS Depth file Particle!");
			return false;
		}

		reader.seekg(0, std::ios::end);
		shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);

		shaderData.assign((std::istreambuf_iterator<char>(reader)),
			std::istreambuf_iterator<char>());

		if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShaderParticle)))
		{
			ErrorLog::Log("Failed to create vertex shader Particle!");
			return false;
		}

		vShaderByteCodeParticle = shaderData;
		shaderData.clear();
		reader.close();

		// [SHADOW] PREPASS:

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

		// [MAIN]:

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

	// GEOMENTRYSHADER:

		// [PARTICLE]:

		reader.open("../x64/Debug/GeometryShaderParticle.cso", std::ios::binary | std::ios::ate);
		if (!reader.is_open())
		{
			ErrorLog::Log("Could not open GS file Particle!");
			return false;
		}

		reader.seekg(0, std::ios::end);
		shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);

		shaderData.assign((std::istreambuf_iterator<char>(reader)),
			std::istreambuf_iterator<char>());

		if (FAILED(device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &gShaderParticle)))
		{
			ErrorLog::Log("Failed to create geomentry shader Particle!");
			return false;
		}

		shaderData.clear();
		reader.close();

	// PIXELSHADER:
	
		// [PARTICLE]:

		reader.open("../x64/Debug/PixelShaderParticle.cso", std::ios::binary | std::ios::ate);
		if (!reader.is_open())
		{
			ErrorLog::Log("Could not open PS file Particle!");
			return false;
		}

		reader.seekg(0, std::ios::end);
		shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);

		shaderData.assign((std::istreambuf_iterator<char>(reader)),
			std::istreambuf_iterator<char>());

		if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShaderParticle)))
		{
			ErrorLog::Log("Failed to create pixel shader Particle!");
			return false;
		}

		shaderData.clear();
		reader.close();

		// [MAIN]:

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

		shaderData.clear();
		reader.close();

	// COMPUTERSHADER:

		// [PARTICLE]:

		reader.open("../x64/Debug/ComputeShaderParticle.cso", std::ios::binary | std::ios::ate);
		if (!reader.is_open()) {
			ErrorLog::Log("Could not open CS Particle file!");
			return false;
		}

		reader.seekg(0, std::ios::end);
		shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);

		shaderData.assign((std::istreambuf_iterator<char>(reader)),
			std::istreambuf_iterator<char>());

		if (FAILED(device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &cShaderParticle))) {
			ErrorLog::Log("Failed to create computer shader particle!");
			return false;
		}

		shaderData.clear();
		reader.close();

		// [MAIN]

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

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayoutVs, ID3D11InputLayout*& inputLayoutVsDepth, 
	ID3D11InputLayout*& inputLayoutVsParticle, const std::string& vShaderByteCode, const std::string& vShaderByteCodeDepth, 
	const std::string& vShaderByteCodeParticle)
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

	D3D11_INPUT_ELEMENT_DESC particleDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	if (FAILED(device->CreateInputLayout(particleDesc, _countof(particleDesc), vShaderByteCodeParticle.c_str(),
		vShaderByteCodeParticle.length(), &inputLayoutVsParticle)))
	{
		ErrorLog::Log("Failed to create Input Layout vs Particle");
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

	if (FAILED(device->CreateSamplerState(&samplerDesc, &sampleState))) {
		ErrorLog::Log("Failed to create sampler state!");
		return false;
	}

	return true;
}

bool CreateSampleStateShadow(ID3D11Device* device, ID3D11SamplerState*& sampleStateShadow)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&samplerDesc, &sampleStateShadow)))
	{
		ErrorLog::Log("Failed to create Sample State Shadow");
		return false;
	}

	return true;
}

bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderDepth, ID3D11VertexShader*& vShaderParticle,
	ID3D11GeometryShader*& gShaderParticle, ID3D11PixelShader*& pShader, ID3D11PixelShader*& pShaderParticle, ID3D11ComputeShader*& cShader,
	ID3D11ComputeShader*& cShaderParticle, ID3D11InputLayout*& inputLayoutVS, ID3D11InputLayout*& inputLayoutVSDepth, 
	ID3D11InputLayout*& inputLayoutVSParticle, ID3D11SamplerState*& sampleState, ID3D11SamplerState*& sampleStateShadow)
{
	std::string vShaderByteCode, vShaderByteCodeDepth, vShaderByteCodeParticle;

	if (!LoadShaders(device, vShader, vShaderDepth, vShaderParticle, gShaderParticle, pShader, pShaderParticle, cShader, cShaderParticle,
		vShaderByteCode, vShaderByteCodeDepth, vShaderByteCodeParticle))
		return false;

	if (!CreateInputLayout(device, inputLayoutVS, inputLayoutVSDepth, inputLayoutVSParticle, vShaderByteCode, vShaderByteCodeDepth,
		vShaderByteCodeParticle))
		return false;

	if (!CreateSampleState(device, sampleState))
		return false;

	if (!CreateSampleStateShadow(device, sampleStateShadow))

	return true;
}
