#pragma once

#include "Setup/WindowHelper.h"
#include "Setup/D3D11Helper.h"
#include "Setup/PipelineHelper.h"
#include "Setup\ShadowHelper.h"
#include "Setup\ParticleHelper.h"
#include "Setup/CubeMappingHelper.h"
#include "Helper/BufferHelper.h"
#include "Scene/modelHelper.h"
#include "Scene/ModelReader/ObjHelper.h"
#include "Camera.h"
#include "QuadTree.h"

#include <d3d11.h>
#include <vector>
#include <string>
#include <directxmath.h>

using namespace DirectX;

namespace getNames
{
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;

	ID3D11UnorderedAccessView* UAView;
	ID3D11UnorderedAccessView* UAViewP;

	ID3D11DepthStencilView* dsView;
	ID3D11DepthStencilView* dsViewShadow[4];
	ID3D11DepthStencilView* dsViewParticle;
	ID3D11ShaderResourceView* SRVShadow[4];
	ID3D11RenderTargetView* gBufferRTV[6];
	ID3D11RenderTargetView* gBufferRTVParticle;
	ID3D11ShaderResourceView* gBufferSRV[6];
	ID3D11ShaderResourceView* gBufferSRVParticle;
	D3D11_VIEWPORT viewport;
	D3D11_VIEWPORT viewportShadow;
	D3D11_VIEWPORT viewportParticle;
	D3D11_VIEWPORT viewportCubeMapping;

	ID3D11VertexShader* vShader;
	ID3D11VertexShader* vShaderDepth;
	ID3D11VertexShader* vShaderParticle;
	ID3D11VertexShader* vShaderMainCubeMapping;
	ID3D11GeometryShader* gShaderParticle;
	ID3D11PixelShader* pShader;
	ID3D11PixelShader* pShaderParticle;
	ID3D11PixelShader* pShaderMainCubeMapping;
	ID3D11ComputeShader* cShader;
	ID3D11ComputeShader* cShaderParticle;

	ID3D11InputLayout* inputLayoutVS;
	ID3D11InputLayout* inputLayoutVSDepth;
	ID3D11InputLayout* inputLayoutVSParticle;
	ID3D11InputLayout* inputLayoutVSMainCubeMapping;

	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11SamplerState* sampleStateCubeMapping;

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* lightBufferTwo;
	ID3D11Buffer* lightBufferThree;
	ID3D11Buffer* lightBufferFour;

	ID3D11Buffer* camBuffer;
	ID3D11Buffer* theWorldBuffer;
	ID3D11Buffer* particleBuffer;
	ID3D11Buffer* directionBuffer;
	ID3D11Buffer* getDTTimeBuffer;

	struct LightData lightData;
	struct LightDataTwo lightDataTwo;
	struct LightDataThree lightDataThree;
	struct LightDataFour lightDataFour;

	struct CamData camData;
	struct TheWorld theWorld;
	struct DepthBufferData depthBufferData;
	struct ParticlePosition particlePosition;
	struct GetDirection getDirection;
	struct GetDtTime getDTTime;

	ID3D11HullShader* hShader;
	ID3D11DomainShader* dShader;
	ID3D11RasterizerState* rasterizerState;

	ID3D11ShaderResourceView* srvCubeMapping;
	ID3D11UnorderedAccessView* UAVCubeMapping[6];
	ID3D11RenderTargetView* gBufferRTVPreCube[3];

	std::vector<std::string> modelName;
	std::vector<XMFLOAT3> worldPos;
}