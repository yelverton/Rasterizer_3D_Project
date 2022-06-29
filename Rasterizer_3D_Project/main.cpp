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

void Render(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, 
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout,
	ID3D11SamplerState* sampleState, ID3D11Buffer* lightBuffer, ID3D11Buffer* camBuffer, ID3D11Buffer* matrixBuffer, 
	struct LightData lightData, struct CamData camData, struct BufferData matrixData, vector<Mesh> mesh, Camera& camera, 
	ID3D11RenderTargetView* gBufferRTV[], vector<XMFLOAT3> worldPos, bool &playerPerspectiv, Camera &lightCamera)
{
	float clearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < 6; i++)
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColour);

	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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
		XMStoreFloat4x4(&matrixData.view, XMMatrixTranspose(camera.GetViewMatrix()));
	} else {
		lightCamera.moveCamera(lightCamera, dt);
		XMStoreFloat4x4(&matrixData.view, XMMatrixTranspose(lightCamera.GetViewMatrix()));
	}

	camData.cameraPosition = camera.GetPositionFloat3();
	lightData.lightPosition = lightCamera.GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subLight = {};
	immediateContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLight);
	memcpy(subLight.pData, &lightData, sizeof(LightData));
	immediateContext->Unmap(lightBuffer, 0);
	immediateContext->CSSetConstantBuffers(0, 1, &lightBuffer);
	
	D3D11_MAPPED_SUBRESOURCE subCam = {};
	immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
	memcpy(subCam.pData, &camData, sizeof(CamData));
	immediateContext->Unmap(camBuffer, 0);
	immediateContext->CSSetConstantBuffers(1, 1, &camBuffer);

	DirectX::XMMATRIX Identity = XMMatrixIdentity();

	for (int i = 0; i < mesh.size(); i++)
	{
		Identity = XMMatrixTranslation(worldPos[i].x, worldPos[i].y, worldPos[i].z);
		XMStoreFloat4x4(&matrixData.world, XMMatrixTranspose(Identity));

		D3D11_MAPPED_SUBRESOURCE subData = {};
		immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
		memcpy(subData.pData, &matrixData, sizeof(BufferData));
		immediateContext->Unmap(matrixBuffer, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

		mesh[i].Draw();
	}

	ID3D11RenderTargetView* nullRTV[6] = { nullptr };
	immediateContext->OMSetRenderTargets(6, nullRTV, nullptr);
}

void RenderComputerShader(ID3D11DeviceContext* immediateContext, ID3D11ComputeShader* cShader, ID3D11DepthStencilView* dsView,
	ID3D11UnorderedAccessView* UAView, ID3D11ShaderResourceView* gBufferSRV[])
{
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
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	ID3D11RenderTargetView* gBufferRTV[6];
	ID3D11ShaderResourceView* gBufferSRV[6];
	D3D11_VIEWPORT viewport;

	ID3D11VertexShader* vShader;
	ID3D11VertexShader* vShaderDepth;
	ID3D11PixelShader* pShader; 
	ID3D11ComputeShader* cShader;

	ID3D11InputLayout* inputLayoutVS;
	ID3D11InputLayout* inputLayoutVSDepth;

	ID3D11SamplerState* sampleState;

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* camBuffer;
	ID3D11Buffer* matrixBuffer;

	struct LightData lightData;
	struct CamData camData;
	struct BufferData matrixData;
	
	std::vector<std::string> modelName;
	std::vector<XMFLOAT3> worldPos;

	std::vector<Mesh> mesh;
	Camera camera;

	Camera lightCamera;
	bool playerPerspectiv = true;
	lightCamera.SetPosition(0.0f, 20.0f, 0.0f);
	lightCamera.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, 
		UAView, dsTexture, dsView, viewport, gBufferRTV, gBufferSRV))
		return -1;

	if (!SetupPipeline(device, vShader, vShaderDepth, pShader, cShader, inputLayoutVS, inputLayoutVSDepth, sampleState))
		return -1;

	if (!SetupBuffers(device, lightBuffer, camBuffer, lightData, camData))
		return -1;

	if (!SetupModels(modelName, worldPos))
		return -1;
		
	for (int i = 0; i < modelName.size(); i++)
		if (!objReader(modelName[i], mesh, device, immediateContext))
			return -1;

	if (!setupModelBuffers(device, matrixBuffer, matrixData, (float)WIDTH, (float)HEIGHT))
		return -1;


	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto start = std::chrono::system_clock::now();
		Render(immediateContext, dsView, viewport, vShader, pShader, inputLayoutVS, sampleState, 
			lightBuffer, camBuffer, matrixBuffer, lightData, camData, matrixData, mesh, camera, 
			gBufferRTV, worldPos, playerPerspectiv, lightCamera);
		RenderComputerShader(immediateContext, cShader, dsView, UAView, gBufferSRV);
		swapChain->Present(0, 0);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		dt = elapsed_seconds.count();
	}

	lightBuffer->Release();
	camBuffer->Release();
	sampleState->Release();
	inputLayoutVS->Release();
	inputLayoutVSDepth->Release();
	pShader->Release();
	vShader->Release();
	vShaderDepth->Release();
	cShader->Release();
	dsView->Release();
	dsTexture->Release();
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
