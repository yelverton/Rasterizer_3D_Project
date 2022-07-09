#include <chrono>
#include <ctime>

#include "Setup/WindowHelper.h"
#include "Setup/D3D11Helper.h"
#include "Setup/PipelineHelper.h"
#include "Helper/BufferHelper.h"
#include "Scene/modelHelper.h"
#include "Scene/ModelReader/ObjHelper.h"
#include "Camera.h"

float dt = 0;

void clearRenderTargetView(ID3D11DeviceContext*& immediateContext, ID3D11DepthStencilView*& dsViewShadow, ID3D11RenderTargetView* gBufferRTV[],
	ID3D11DepthStencilView*& dsView)
{
	float clearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < 6; i++)
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColour);

	immediateContext->ClearDepthStencilView(dsViewShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ShadowPrePass(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView*& dsViewShadow, D3D11_VIEWPORT& viewportShadow, Camera &lightCamera,
	vector<Mesh> mesh, ID3D11VertexShader* vShaderDepth, ID3D11InputLayout* inputLayoutVSDepth, ID3D11SamplerState* sampleStateShadow)
{
	immediateContext->IASetInputLayout(inputLayoutVSDepth);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShaderDepth, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewportShadow);

	ID3D11PixelShader* nullPixelShader = nullptr;
	immediateContext->PSSetShader(nullPixelShader, nullptr, 0);
	immediateContext->PSGetSamplers(1, 1, &sampleStateShadow);
	immediateContext->OMSetRenderTargets(0, nullptr, dsViewShadow);

	lightCamera.sendViewProjection(lightCamera, 1);
}

void drawPrePass(ID3D11DeviceContext* immediateContext, vector<Mesh>& mesh, vector<XMFLOAT3> worldPos, struct TheWorld theWorld, ID3D11Buffer* theWorldBuffer)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();

	for (int i = 0; i < mesh.size(); i++)
	{
		Identity = XMMatrixTranslation(worldPos[i].x, worldPos[i].y, worldPos[i].z);
		XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

		D3D11_MAPPED_SUBRESOURCE subData = {};
		immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
		std::memcpy(subData.pData, &theWorld, sizeof(BufferData));
		immediateContext->Unmap(theWorldBuffer, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);

		mesh[i].DrawPrePass();
	}
}

void Render(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, 
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampleState, 
	ID3D11Buffer* lightBuffer, ID3D11Buffer* camBuffer, struct LightData lightData, struct CamData camData, Camera& camera,
	ID3D11RenderTargetView* gBufferRTV[], bool &playerPerspectiv, Camera &lightCamera, ID3D11ShaderResourceView*& SRVShadow)
{
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(6, gBufferRTV, dsView);
	immediateContext->PSSetSamplers(0, 1, &sampleState);

	if (GetAsyncKeyState('X'))
		playerPerspectiv = true;
	else if (GetAsyncKeyState('C'))
		playerPerspectiv = false;

	if (playerPerspectiv) {
		camera.moveCamera(camera, dt);
		camera.sendViewProjection(camera, 1);
	} else {
		lightCamera.moveCamera(lightCamera, dt);
		lightCamera.sendViewProjection(lightCamera, 1);	
	}

	//lightCamera.SetLookAtPos(camera.GetPositionFloat3());
	camData.cameraPosition = camera.GetPositionFloat3();
	lightData.lightPosition = lightCamera.GetPositionFloat3()/*XMFLOAT3(-lightCamera.GetPositionFloat3().x, lightCamera.GetPositionFloat3().y, -lightCamera.GetPositionFloat3().z)*/;

	D3D11_MAPPED_SUBRESOURCE subLight = {};
	immediateContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLight);
	std::memcpy(subLight.pData, &lightData, sizeof(LightData));
	immediateContext->Unmap(lightBuffer, 0);
	immediateContext->CSSetConstantBuffers(0, 1, &lightBuffer);
	
	D3D11_MAPPED_SUBRESOURCE subCam = {};
	immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
	std::memcpy(subCam.pData, &camData, sizeof(CamData));
	immediateContext->Unmap(camBuffer, 0);
	immediateContext->CSSetConstantBuffers(1, 1, &camBuffer);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow);
}

void draw(ID3D11DeviceContext* immediateContext, vector<Mesh>& mesh, vector<XMFLOAT3> worldPos, struct TheWorld theWorld, ID3D11Buffer* theWorldBuffer,
	Camera& camera, bool& playerPerspectiv, Camera& lightCamera)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();

	for (int i = 0; i < mesh.size(); i++)
	{
		Identity = XMMatrixTranslation(worldPos[i].x, worldPos[i].y, worldPos[i].z);
		XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

		D3D11_MAPPED_SUBRESOURCE subData = {};
		immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
		std::memcpy(subData.pData, &theWorld, sizeof(BufferData));
		immediateContext->Unmap(theWorldBuffer, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);

		lightCamera.sendViewProjection(lightCamera, 2);

		mesh[i].Draw();
	}
}

void RenderComputerShader(ID3D11DeviceContext* immediateContext, ID3D11ComputeShader* cShader, ID3D11DepthStencilView* dsView,
	ID3D11UnorderedAccessView* UAView, ID3D11ShaderResourceView* gBufferSRV[])
{
	ID3D11RenderTargetView* nullRTV[6] = { nullptr };
	immediateContext->OMSetRenderTargets(6, nullRTV, nullptr);

	immediateContext->CSSetShader(cShader, nullptr, 0);
	immediateContext->CSSetShaderResources(0, 6, gBufferSRV);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &UAView, nullptr);
	
	immediateContext->Dispatch(32, 32, 1);

	ID3D11UnorderedAccessView* NullUAV = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &NullUAV, nullptr);
	ID3D11ShaderResourceView* gBufferSRVNULL[6] = { nullptr };
	immediateContext->CSSetShaderResources(0, 6, gBufferSRVNULL);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;

	HWND window;
	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
		return -1;

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11UnorderedAccessView* UAView;
	ID3D11DepthStencilView* dsView;
	ID3D11DepthStencilView* dsViewShadow;
	ID3D11ShaderResourceView* SRVShadow;
	ID3D11RenderTargetView* gBufferRTV[6];
	ID3D11ShaderResourceView* gBufferSRV[6];
	D3D11_VIEWPORT viewport;
	D3D11_VIEWPORT viewportShadow;

	ID3D11VertexShader* vShader;
	ID3D11VertexShader* vShaderDepth;
	ID3D11PixelShader* pShader; 
	ID3D11ComputeShader* cShader;

	ID3D11InputLayout* inputLayoutVS;
	ID3D11InputLayout* inputLayoutVSDepth;

	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* camBuffer;
	ID3D11Buffer* theWorldBuffer;

	struct LightData lightData;
	struct CamData camData;
	struct TheWorld theWorld;
	struct DepthBufferData depthBufferData;
	
	std::vector<std::string> modelName;
	std::vector<XMFLOAT3> worldPos;

	std::vector<Mesh> mesh;
	Camera camera;
	Camera lightCamera;

	bool playerPerspectiv = true;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, UAView, dsView, dsViewShadow, viewport,
		viewportShadow, SRVShadow, gBufferRTV, gBufferSRV))
		return -1;

	if (!SetupPipeline(device, vShader, vShaderDepth, pShader, cShader, inputLayoutVS, inputLayoutVSDepth, sampleState, 
		sampleStateShadow))
		return -1;

	if (!SetupBuffers(device, lightBuffer, camBuffer, theWorldBuffer, lightData, camData, theWorld))
		return -1;

	if (!SetupModels(modelName, worldPos))
		return -1;
		
	for (int i = 0; i < modelName.size(); i++)
		if (!objReader(modelName[i], mesh, device, immediateContext))
			return -1;

	camera.createConstantBuffer(device, immediateContext);
	lightCamera.createConstantBuffer(device, immediateContext);

	lightCamera.AdjustPosition(0.0f, 30.0f, 0.0f);
	//lightCamera.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
	lightCamera.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);

	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto start = std::chrono::system_clock::now();
		clearRenderTargetView(immediateContext, dsViewShadow, gBufferRTV, dsView);
		ShadowPrePass(immediateContext, dsViewShadow, viewportShadow, lightCamera, mesh, vShaderDepth, inputLayoutVSDepth, sampleStateShadow);
		drawPrePass(immediateContext, mesh, worldPos, theWorld, theWorldBuffer);
		Render(immediateContext, dsView, viewport, vShader, pShader, inputLayoutVS, sampleState, 
			lightBuffer, camBuffer, lightData, camData, camera, gBufferRTV, playerPerspectiv, lightCamera, SRVShadow);
		draw(immediateContext, mesh, worldPos, theWorld, theWorldBuffer, camera, playerPerspectiv, lightCamera);
		RenderComputerShader(immediateContext, cShader, dsView, UAView, gBufferSRV);
		swapChain->Present(0, 0);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		dt = elapsed_seconds.count();
	}

	lightBuffer->Release();
	camBuffer->Release();
	sampleState->Release();
	sampleState->Release();
	inputLayoutVS->Release();
	inputLayoutVSDepth->Release();
	pShader->Release();
	vShader->Release();
	vShaderDepth->Release();
	cShader->Release();
	dsView->Release();
	dsViewShadow->Release();
	SRVShadow->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();

	for (int i = 0; i < 6; i++)
	{
		gBufferRTV[i]->Release();
		gBufferSRV[i]->Release();
	}

	return 0;
}
