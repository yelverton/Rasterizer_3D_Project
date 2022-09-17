#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxmath.h>
using namespace DirectX;

class PipelineHelper
{
protected:
	bool Reader(std::string type, std::string& shaderData);

public:
	PipelineHelper(ID3D11Device* d, std::string r = "");
	~PipelineHelper();

	bool VSet(ID3D11VertexShader*& vShader, ID3D11InputLayout*& inputLayoutVS);
	bool HSet(ID3D11HullShader*& hShader);
	bool DSet(ID3D11DomainShader*& dShader);
	bool RSet(ID3D11RasterizerState*& rasterizerState);
	bool GSet(ID3D11GeometryShader*& gShader);
	bool PSet(ID3D11PixelShader*& pShader);
	bool CSet(ID3D11ComputeShader*& cShader);

	bool SetupSampleShadowShaders(ID3D11SamplerState*& sampleStateShadow);
	bool SetupSampleStateCubeMapping(ID3D11SamplerState*& sampleState);

private:
	ID3D11Device* device;
	std::string name;
};