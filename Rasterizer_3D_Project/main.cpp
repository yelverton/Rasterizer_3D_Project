#include <chrono>
#include <ctime>

#include "Setup/WindowHelper.h"
#include "Setup/D3D11Helper.h"
#include "Setup/PipelineHelper.h"
#include "Setup\ShadowHelper.h"
#include "Setup\ParticleHelper.h"
#include "Helper/BufferHelper.h"
#include "Scene/modelHelper.h"
#include "Scene/ModelReader/ObjHelper.h"
#include "Camera.h"

float dt = 0;

void clearRenderTargetView(ID3D11DeviceContext*& immediateContext, ID3D11DepthStencilView*& dsViewShadow, ID3D11RenderTargetView* gBufferRTV[],
	ID3D11DepthStencilView*& dsView, ID3D11DepthStencilView*& dsViewParticle, ID3D11RenderTargetView* rtv)
{
	float clearColour[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
	for (int i = 0; i < 6; i++)
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColour);

	immediateContext->ClearRenderTargetView(rtv, clearColour);
	
	immediateContext->ClearDepthStencilView(dsViewParticle, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediateContext->ClearDepthStencilView(dsViewShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void moveAbility(bool& playerPerspectiv, Camera& lightCamera, Camera& camera, Camera& shadow)
{
	if (GetAsyncKeyState('X'))
		playerPerspectiv = true;
	else if (GetAsyncKeyState('C'))
		playerPerspectiv = false;

	if (playerPerspectiv) {
		camera.moveCamera(camera, dt);
		/*shadow.moveCamera(shadow, dt);*/
	}
	else {
		lightCamera.moveCamera(lightCamera, dt);
	}
}

void ShadowPrePass(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView*& dsViewShadow, D3D11_VIEWPORT viewportShadow, Camera lightCamera,
	ID3D11VertexShader*& vShaderDepth, ID3D11InputLayout* inputLayoutShadow, ID3D11SamplerState* sampleStateShadow, ID3D11RenderTargetView* rtv)
{
	immediateContext->IASetInputLayout(inputLayoutShadow);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShaderDepth, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewportShadow);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
	immediateContext->OMSetRenderTargets(0, nullptr, dsViewShadow);

	lightCamera.sendViewProjection(lightCamera, 1);
}

void drawPrePass(ID3D11DeviceContext* immediateContext, vector<Mesh> mesh, vector<XMFLOAT3> worldPos, struct TheWorld theWorld, ID3D11Buffer* theWorldBuffer)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();
	for (int i = 0; i < mesh.size() - 1; i++)
	{
		Identity = XMMatrixTranslation(worldPos[i].x, worldPos[i].y, worldPos[i].z);
		XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

		D3D11_MAPPED_SUBRESOURCE subData = {};
		immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
		std::memcpy(subData.pData, &theWorld, sizeof(TheWorld));
		immediateContext->Unmap(theWorldBuffer, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);

		mesh[i].DrawPrePass();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
}

void Render(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT viewport, 
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampleState, 
	ID3D11Buffer* lightBuffer, ID3D11Buffer* camBuffer, struct LightData lightData, struct CamData camData, Camera& camera,
	ID3D11RenderTargetView* gBufferRTV[], bool &playerPerspectiv, Camera &lightCamera, ID3D11ShaderResourceView* SRVShadow,
	ID3D11SamplerState* sampleStateShadow, Camera &shadow)
{
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(6, gBufferRTV, dsView);
	immediateContext->PSSetSamplers(0, 1, &sampleState);
	immediateContext->PSSetSamplers(1, 1, &sampleStateShadow);

	playerPerspectiv ? camera.sendViewProjection(camera, 1) : lightCamera.sendViewProjection(lightCamera, 1);

	lightCamera.sendViewProjection(lightCamera, 2);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow);
}

void draw(ID3D11DeviceContext* immediateContext, vector<Mesh>& mesh, vector<XMFLOAT3> worldPos, struct TheWorld theWorld, ID3D11Buffer* theWorldBuffer,
	Camera& camera, bool& playerPerspectiv, Camera& lightCamera)
{
	for (int i = 0; i < mesh.size(); i++)
	{
		DirectX::XMMATRIX Identity = XMMatrixIdentity();
		Identity = XMMatrixTranslation(worldPos[i].x, worldPos[i].y, worldPos[i].z);
		XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

		D3D11_MAPPED_SUBRESOURCE subData = {};
		immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
		std::memcpy(subData.pData, &theWorld, sizeof(TheWorld));
		immediateContext->Unmap(theWorldBuffer, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);

		mesh[i].Draw();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
}

void RenderComputerShader(ID3D11DeviceContext* immediateContext, ID3D11ComputeShader* cShader, ID3D11DepthStencilView* dsView,
	ID3D11UnorderedAccessView* UAView, ID3D11ShaderResourceView* gBufferSRV[], CamData& camData, Camera& camera, LightData lightData,
	Camera& lightCamera, ID3D11Buffer* lightBuffer, ID3D11Buffer* camBuffer)
{
	ID3D11RenderTargetView* nullRTV[6] = { nullptr };
	immediateContext->OMSetRenderTargets(6, nullRTV, nullptr);

	immediateContext->CSSetShader(cShader, nullptr, 0);
	immediateContext->CSSetShaderResources(0, 6, gBufferSRV);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &UAView, nullptr);

	//camData.cameraPosition = camera.GetPositionFloat3();
	//lightData.lightPosition = lightCamera.GetPositionFloat3();

	//D3D11_MAPPED_SUBRESOURCE subLight = {};
	//immediateContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLight);
	//std::memcpy(subLight.pData, &lightData, sizeof(LightData));
	//immediateContext->Unmap(lightBuffer, 0);
	//immediateContext->CSSetConstantBuffers(0, 1, &lightBuffer);

	//D3D11_MAPPED_SUBRESOURCE subCam = {};
	//immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
	//std::memcpy(subCam.pData, &camData, sizeof(CamData));
	//immediateContext->Unmap(camBuffer, 0);
	//immediateContext->CSSetConstantBuffers(1, 1, &camBuffer);
	
	immediateContext->Dispatch(32, 32, 1);

	ID3D11UnorderedAccessView* NullUAV = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &NullUAV, nullptr);
	ID3D11ShaderResourceView* gBufferSRVNULL[6] = { nullptr };
	immediateContext->CSSetShaderResources(0, 6, gBufferSRVNULL);

	immediateContext->CSSetShader(nullptr, nullptr, 0);
}

void particleSystem(ID3D11DeviceContext* immediateContext, ID3D11InputLayout* inputLayoutVSParticle, ID3D11VertexShader* vShaderParticle,
	ID3D11DepthStencilView* dsViewParticle, D3D11_VIEWPORT& viewportParticle, ID3D11GeometryShader* gShaderParticle, ID3D11PixelShader* pShaderParticle, ID3D11ComputeShader* cShaderParticle,
	ID3D11Buffer* getDirectionBuffer, struct GetDirection getDirection, Camera& camera, ID3D11Buffer* camBuffer, struct CamData camData,
	ID3D11RenderTargetView*& rtv)
{
	immediateContext->IASetInputLayout(inputLayoutVSParticle);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	immediateContext->VSSetShader(vShaderParticle, nullptr, 0);
	immediateContext->OMSetRenderTargets(1, &rtv, dsViewParticle);
	immediateContext->RSSetViewports(1, &viewportParticle);

	immediateContext->GSSetShader(gShaderParticle, nullptr, 0);
	immediateContext->PSSetShader(pShaderParticle, nullptr, 0);

	XMStoreFloat3(&getDirection.forwardVec, camera.GetForwardVector());
	XMStoreFloat3(&getDirection.upVec, camera.GetUpVector());;

	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(getDirectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	std::memcpy(subData.pData, &getDirection, sizeof(getDirection));
	immediateContext->Unmap(getDirectionBuffer, 0);

	immediateContext->GSSetConstantBuffers(0, 1, &getDirectionBuffer);
	camera.sendViewProjectionGS(camera, 1);
}

void drawParticle(ID3D11DeviceContext* immediateContext, vector<XMFLOAT3>& particle, vector<XMFLOAT3> worldPos, struct TheWorld theWorld, ID3D11Buffer* theWorldBuffer,
	ID3D11Buffer* particleBuffer, ID3D11DepthStencilView* dsView)
{
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;

	DirectX::XMMATRIX Identity = XMMatrixIdentity();
	XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	std::memcpy(subData.pData, &theWorld, sizeof(TheWorld));
	immediateContext->Unmap(theWorldBuffer, 0);
	immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);

	immediateContext->IASetVertexBuffers(0, 1, &particleBuffer, &stride, &offset);
	immediateContext->Draw(particle.size(), 0);

	immediateContext->GSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
	immediateContext->VSSetShader(nullptr, nullptr, 0);
}

void RenderComputerShaderParticle(ID3D11DeviceContext* immediateContext, ID3D11ComputeShader*& cShaderParticle, 
	ID3D11RenderTargetView*& gBufferRTVParticle, ID3D11ShaderResourceView*& gBufferSRVParticle,
	ID3D11UnorderedAccessView*& UAViewP, vector<XMFLOAT3> particle, ID3D11Buffer*& getDtTimeBuffer, struct GetDtTime& getDtTime)
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	immediateContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	immediateContext->CSSetShader(cShaderParticle, nullptr, 0);
	immediateContext->CSSetShaderResources(0, 1, &gBufferSRVParticle);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &UAViewP, nullptr);

	getDtTime.dt += dt;
	if (getDtTime.dt > 100.0f)
		getDtTime.dt = 0.0f;

	D3D11_MAPPED_SUBRESOURCE getDtTImeRes = {};
	immediateContext->Map(getDtTimeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &getDtTImeRes);
	std::memcpy(getDtTImeRes.pData, &getDtTime, sizeof(GetDtTime));
	immediateContext->Unmap(getDtTimeBuffer, 0);
	immediateContext->CSSetConstantBuffers(0, 1, &getDtTimeBuffer);

	immediateContext->Dispatch(particle.size(), 1, 1);

	ID3D11UnorderedAccessView* NullUAV = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &NullUAV, nullptr);
	ID3D11ShaderResourceView* gBufferSRVNULL = nullptr;
	immediateContext->CSSetShaderResources(0, 1, &gBufferSRVNULL);

	immediateContext->CSSetShader(nullptr, nullptr, 0);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 1024;

	HWND window;
	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
		return -1;

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;

	ID3D11UnorderedAccessView* UAView;
	ID3D11UnorderedAccessView* UAViewP;

	ID3D11DepthStencilView* dsView;
	ID3D11DepthStencilView* dsViewShadow;
	ID3D11DepthStencilView* dsViewParticle;
	ID3D11ShaderResourceView* SRVShadow;
	ID3D11RenderTargetView* gBufferRTV[6];
	ID3D11RenderTargetView* gBufferRTVParticle;
	ID3D11ShaderResourceView* gBufferSRV[6];
	ID3D11ShaderResourceView* gBufferSRVParticle;
	D3D11_VIEWPORT viewport;
	D3D11_VIEWPORT viewportShadow;
	D3D11_VIEWPORT viewportParticle;

	ID3D11VertexShader* vShader;
	ID3D11VertexShader* vShaderDepth;
	ID3D11VertexShader* vShaderParticle;
	ID3D11GeometryShader* gShaderParticle;
	ID3D11PixelShader* pShader; 
	ID3D11PixelShader* pShaderParticle;
	ID3D11ComputeShader* cShader;
	ID3D11ComputeShader* cShaderParticle;

	ID3D11InputLayout* inputLayoutVS;
	ID3D11InputLayout* inputLayoutVSDepth;
	ID3D11InputLayout* inputLayoutVSParticle;

	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11SamplerState* sampleStateParticle;

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* camBuffer;
	ID3D11Buffer* theWorldBuffer;
	ID3D11Buffer* particleBuffer;
	ID3D11Buffer* directionBuffer;
	ID3D11Buffer* getDTTimeBuffer;

	struct LightData lightData;
	struct CamData camData;
	struct TheWorld theWorld;
	struct DepthBufferData depthBufferData;
	struct ParticlePosition particlePosition;
	struct GetDirection getDirection;
	struct GetDtTime getDTTime;
	
	std::vector<std::string> modelName;
	std::vector<XMFLOAT3> worldPos;

	std::vector<Mesh> mesh;
	Camera camera;
	Camera lightCamera;
	Camera shadow;

	std::vector<XMFLOAT3> particels;

	bool playerPerspectiv = true;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsView, viewport))
		return -1;

	if (!SetupShadowHelper(device, immediateContext, viewportShadow, WIDTH, HEIGHT, dsViewShadow, SRVShadow,
		gBufferRTV, gBufferSRV, swapChain, UAView))
		return -1;

	if (!SetupParticleHelper(device, immediateContext, UAViewP, swapChain, particleBuffer, particels, particlePosition,
		WIDTH, HEIGHT, dsViewParticle, viewportParticle, gBufferRTVParticle, gBufferSRVParticle))
		return -1;

	if (!SetupPipeline(device, vShader, vShaderDepth, vShaderParticle, gShaderParticle, pShader, pShaderParticle, cShader,
		cShaderParticle, inputLayoutVS, inputLayoutVSDepth, inputLayoutVSParticle, sampleState, sampleStateShadow,
		sampleStateParticle))
		return -1;

	if (!SetupBuffers(device, lightBuffer, camBuffer, theWorldBuffer, directionBuffer, getDTTimeBuffer, lightData, camData, 
		theWorld, getDirection, getDTTime))
		return -1;

	if (!SetupModels(modelName, worldPos))
		return -1;
		
	for (int i = 0; i < modelName.size(); i++)
		if (!objReader(modelName[i], mesh, device, immediateContext))
			return -1;

	camera.createConstantBuffer(camera, device, immediateContext);
	lightCamera.createConstantBuffer(camera, device, immediateContext);
	shadow.createConstantBuffer(camera, device, immediateContext);

	lightCamera.AdjustPosition(0.0f, 10.0f, -5.0f);
	lightCamera.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
	lightCamera.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);

	shadow.AdjustPosition(0, 0, 0);
	shadow.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);

	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto start = std::chrono::system_clock::now();
		clearRenderTargetView(immediateContext, dsViewShadow, gBufferRTV, dsView, dsViewParticle, rtv);
		moveAbility(playerPerspectiv, lightCamera, camera, shadow);
		ShadowPrePass(immediateContext, dsViewShadow, viewportShadow, lightCamera, vShaderDepth, inputLayoutVSDepth, sampleStateShadow, rtv);
		drawPrePass(immediateContext, mesh, worldPos, theWorld, theWorldBuffer);
		Render(immediateContext, dsView, viewport, vShader, pShader, inputLayoutVS, sampleState, lightBuffer, camBuffer, lightData, camData, 
			camera, gBufferRTV, playerPerspectiv, lightCamera, SRVShadow, sampleStateShadow, shadow);
		draw(immediateContext, mesh, worldPos, theWorld, theWorldBuffer, camera, playerPerspectiv, lightCamera);
		RenderComputerShader(immediateContext, cShader, dsView, UAView, gBufferSRV, camData, camera, lightData, lightCamera, lightBuffer, 
			camBuffer);
		particleSystem(immediateContext, inputLayoutVSParticle, vShaderParticle, dsView, viewportParticle, gShaderParticle, pShaderParticle, 
			cShaderParticle, directionBuffer, getDirection, camera, camBuffer, camData, rtv);
		drawParticle(immediateContext, particels, worldPos, theWorld, theWorldBuffer, particleBuffer, dsView);
		RenderComputerShaderParticle(immediateContext, cShaderParticle, gBufferRTVParticle, gBufferSRVParticle, UAViewP,
			particels, getDTTimeBuffer, getDTTime);
		swapChain->Present(0, 0);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		dt = elapsed_seconds.count();
	}

	lightBuffer->Release();
	camBuffer->Release();
	particleBuffer->Release();
	sampleState->Release();
	sampleState->Release();
	inputLayoutVS->Release();
	inputLayoutVSDepth->Release();
	inputLayoutVSParticle->Release();
	pShader->Release();
	vShader->Release();
	vShaderDepth->Release();
	vShaderParticle->Release();
	gShaderParticle->Release();
	cShader->Release();
	cShaderParticle->Release();
	dsView->Release();
	dsViewShadow->Release();
	inputLayoutVSParticle->Release();
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
