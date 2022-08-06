#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxmath.h>
using namespace DirectX;


bool SetupShadowShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11InputLayout*& inputLayoutVS);
bool SetupCubeMappingShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11InputLayout*& inputLayoutVS,
	ID3D11PixelShader*& pShader);
bool SetupRenderShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11InputLayout*& inputLayoutVS,
	ID3D11HullShader*& hShader, ID3D11DomainShader*& dShader, ID3D11PixelShader*& pShader,
	ID3D11ComputeShader*& cShader);
bool SetupParticleShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11InputLayout*& inputLayoutVS,
	ID3D11GeometryShader*& gShader, ID3D11PixelShader*& pShader, ID3D11ComputeShader*& cShader);
bool SetupSampleShaders(ID3D11Device* device, ID3D11SamplerState*& sampleState);
bool SetupSampleShadowShaders(ID3D11Device* device, ID3D11SamplerState*& sampleStateShadow);
bool SetupRasterizerState(ID3D11Device* device, ID3D11RasterizerState*& rasterizerState);
bool SetupSampleStateCubeMapping(ID3D11Device* device, ID3D11SamplerState*& sampleState);