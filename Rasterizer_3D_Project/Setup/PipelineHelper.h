#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxmath.h>
using namespace DirectX;

bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderDepth, ID3D11VertexShader*& vShaderParticle,
	ID3D11GeometryShader*& gShaderParticle, ID3D11PixelShader*& pShader, ID3D11PixelShader*& pShaderParticle, ID3D11ComputeShader*& cShader,
	ID3D11ComputeShader*& cShaderParticle, ID3D11InputLayout*& inputLayoutVS, ID3D11InputLayout*& inputLayoutVSDepth,
	ID3D11InputLayout*& inputLayoutVSParticle, ID3D11SamplerState*& sampleState, ID3D11SamplerState*& sampleStateShadow,
	ID3D11SamplerState*& sampleStateParticle, ID3D11HullShader*& hShader, ID3D11DomainShader*& dShader,
	ID3D11RasterizerState*& rasterizerState);