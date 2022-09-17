#pragma once
#include "PipelineHelper.h"

#include <Windows.h>
#include <d3d11.h>
#include <iostream>

class MainRender
{
public:
	MainRender(UINT w, UINT h);
	bool Initilize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, 
		IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView);
	void Draw();
protected:
	bool CreateGBuffers();
	bool CreateUnorderedAccessView(IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView);
private:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	ID3D11RenderTargetView* gBufferRTV[6];
	ID3D11ShaderResourceView* gBufferSRV[6];
	UINT WIDTH;
	UINT HEIGHT;
	PipelineHelper* p;
	ID3D11VertexShader* vShader;
	ID3D11InputLayout* inputLayout;
	ID3D11HullShader* hShader;
	ID3D11DomainShader* dShader;
	ID3D11RasterizerState* rasterizerState;
	ID3D11GeometryShader* gShader;
	ID3D11PixelShader* pShader;
	ID3D11ComputeShader* cShader;
};