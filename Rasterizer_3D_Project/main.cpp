#include <chrono>
#include <ctime>

#include "Id3d11.h"

using namespace getNames;

float dt = 0;

void clearBig()
{
	float clearColour[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

	for (int i = 0; i < 6; i++)
	{
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColour);
	}
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	immediateContext->ClearDepthStencilView(dsViewParticle, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (int i = 0; i < 4; i++)
	{
		immediateContext->ClearDepthStencilView(dsViewShadow[i], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void smallClear()
{
	float clearColour[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

	for (int i = 0; i < 6; i++)
	{
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColour);
	}

	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void setCubeMappingRotation(Camera& cubeMappingCamera, int index)
{
	if (index == 0) cubeMappingCamera.SetRotation(0.0f, XM_PIDIV2, 0.0f);
	if (index == 1) cubeMappingCamera.SetRotation(0.0f, -XM_PIDIV2, 0.0f);
	if (index == 2) cubeMappingCamera.SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	if (index == 3) cubeMappingCamera.SetRotation(XM_PIDIV2, 0.0f, 0.0f);
	if (index == 4) cubeMappingCamera.SetRotation(0.0f, 0.0f, 0.0f);
	if (index == 5) cubeMappingCamera.SetRotation(0.0f, -XM_PI, 0.0f);
}


void moveAbility(int& playerPerspectiv, std::vector<Camera>& camera)
{
	if (GetAsyncKeyState('X'))
		playerPerspectiv = 0;
	else if (GetAsyncKeyState('C'))
		playerPerspectiv = 1;
	else if (GetAsyncKeyState('V'))
		playerPerspectiv = 2;
	else if (GetAsyncKeyState('B'))
		playerPerspectiv = 3;
	else if (GetAsyncKeyState('N'))
		playerPerspectiv = 4;

	camera[playerPerspectiv].moveCamera(camera[playerPerspectiv], dt);
} 

void ShadowPrePass(std::vector<Camera>& camera, int index)
{
	immediateContext->IASetInputLayout(inputLayoutVSDepth);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShaderDepth, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewportShadow);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
	immediateContext->OMSetRenderTargets(0, nullptr, dsViewShadow[index]);

	camera[index + 1].sendViewProjection(camera[index + 1], 1);
}

void drawPrePass(ID3D11DeviceContext* immediateContext, vector<Mesh> mesh)
{
	for (int i = 0; i < mesh.size(); i++)
	{
		mesh[i].Draw();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
}

void RenderPreCube(std::vector<Camera>& camera, int index)
{
	setCubeMappingRotation(camera[5], index);
	immediateContext->IASetInputLayout(inputLayoutVS);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);

	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(6, gBufferRTV, dsView);
	immediateContext->PSSetSamplers(0, 1, &sampleState);
	immediateContext->PSSetSamplers(1, 1, &sampleStateShadow);

	for (int i = 0; i < 5; i++)
		camera[i].sendViewProjection(camera[i], i+1);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow[0]);
	immediateContext->PSSetShaderResources(4, 1, &SRVShadow[1]);
	immediateContext->PSSetShaderResources(5, 1, &SRVShadow[2]);
	immediateContext->PSSetShaderResources(6, 1, &SRVShadow[3]);
}

void Render(std::vector<Camera>& camera, int playerPerspectiv)
{
	immediateContext->IASetInputLayout(inputLayoutVS);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);

	camData.cameraPosition = camera[0].GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subCam = {};
	immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
	std::memcpy(subCam.pData, &camData, sizeof(CamData));
	immediateContext->Unmap(camBuffer, 0);
	immediateContext->HSSetConstantBuffers(0, 1, &camBuffer);

	immediateContext->HSSetShader(hShader, nullptr, 0);
	immediateContext->DSSetShader(dShader, nullptr, 0);
	immediateContext->RSSetState(rasterizerState);

	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(6, gBufferRTV, dsView);
	immediateContext->PSSetSamplers(0, 1, &sampleState);
	immediateContext->PSSetSamplers(1, 1, &sampleStateShadow);

	camera[playerPerspectiv].sendViewProjection(camera[playerPerspectiv], 1);

	for (int i = 1; i < 5; ++i)
		camera[i].sendViewProjection(camera[i], i+1);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow[0]);
	immediateContext->PSSetShaderResources(4, 1, &SRVShadow[1]);
	immediateContext->PSSetShaderResources(5, 1, &SRVShadow[2]);
	immediateContext->PSSetShaderResources(6, 1, &SRVShadow[3]);

}

void draw(vector<Mesh>& mesh, std::vector<int> viewFrustom)
{
	for (int i = 0; i < viewFrustom.size(); i++)
	{
		if (viewFrustom[i] != 0)
			mesh[viewFrustom[i]].Draw();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->HSSetShader(nullptr, nullptr, 0);
	immediateContext->DSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
}

void drawPreCube(vector<Mesh>& mesh, std::vector<int> viewFrustom)
{
	for (int i = 0; i < viewFrustom.size(); i++)
	{
		if (viewFrustom[i] != 0)
			mesh[viewFrustom[i]].Draw();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
}

void RenderComputerShader(std::vector<Camera>& camera)
{
	ID3D11RenderTargetView* nullRTV[6] = { nullptr };
	immediateContext->OMSetRenderTargets(6, nullRTV, nullptr);

	immediateContext->CSSetShader(cShader, nullptr, 0);
	immediateContext->CSSetShaderResources(0, 6, gBufferSRV);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &UAView, nullptr);

	camData.cameraPosition = camera[0].GetPositionFloat3();
	lightData.lightDirection = XMFLOAT3(-0.0f, -0.5f, -0.5f);

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
	
	// other lights
	lightDataTwo.posTwo = camera[2].GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subLightTwo = {};
	immediateContext->Map(lightBufferTwo, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightTwo);
	std::memcpy(subLightTwo.pData, &lightDataTwo, sizeof(LightDataTwo));
	immediateContext->Unmap(lightBufferTwo, 0);
	immediateContext->CSSetConstantBuffers(2, 1, &lightBufferTwo);

	lightDataThree.posThree = camera[3].GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subLightThree = {};
	immediateContext->Map(lightBufferThree, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightThree);
	std::memcpy(subLightThree.pData, &lightDataThree, sizeof(LightDataThree));
	immediateContext->Unmap(lightBufferThree, 0);
	immediateContext->CSSetConstantBuffers(3, 1, &lightBufferThree);

	lightDataFour.posFour = camera[4].GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subLightFour = {};
	immediateContext->Map(lightBufferFour, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightFour);
	std::memcpy(subLightFour.pData, &lightDataFour, sizeof(LightDataFour));
	immediateContext->Unmap(lightBufferFour, 0);
	immediateContext->CSSetConstantBuffers(4, 1, &lightBufferFour);

	immediateContext->Dispatch(32, 32, 1);

	ID3D11UnorderedAccessView* NullUAV = nullptr;
	immediateContext->CSSetUnorderedAccessViews(0, 1, &NullUAV, nullptr);
	ID3D11ShaderResourceView* gBufferSRVNULL[6] = { nullptr };
	immediateContext->CSSetShaderResources(0, 6, gBufferSRVNULL);

	immediateContext->CSSetShader(nullptr, nullptr, 0);
}

void RenderPreCubeMapping(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT viewport,
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampleState,
	Camera& camera, ID3D11RenderTargetView* gBufferRTV[], ID3D11SamplerState* sampleStateShadow)
{
	float clearColour[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	for (int i = 0; i < 3; i++)
		immediateContext->ClearRenderTargetView(gBufferRTV[i], clearColour);

	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);

	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampleState);
	immediateContext->OMSetRenderTargets(3, gBufferRTV, dsView);
}

void cubeMappingSystem(std::vector<Camera>&camera)
{
	immediateContext->IASetInputLayout(inputLayoutVSMainCubeMapping);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShaderMainCubeMapping, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewportCubeMapping);
	immediateContext->PSSetShader(pShaderMainCubeMapping, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampleStateCubeMapping);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);

	camera[5].sendViewProjection(camera[5], 1);

	immediateContext->PSSetShaderResources(0, 1, &srvCubeMapping);

	camData.cameraPosition = camera[5].GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subCam = {};
	immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
	std::memcpy(subCam.pData, &camData, sizeof(CamData));
	immediateContext->Unmap(camBuffer, 0);
	immediateContext->PSSetConstantBuffers(0, 1, &camBuffer);
}

void drawCubeMapping(vector<Mesh>& mesh, std::vector<int> viewFrustom)
{
	mesh[0].DrawPrePass();
	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
}

void particleSystem(std::vector<Camera>&camera)
{
	immediateContext->IASetInputLayout(inputLayoutVSParticle);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	immediateContext->VSSetShader(vShaderParticle, nullptr, 0);
	immediateContext->OMSetRenderTargets(1, &rtv, dsViewParticle);
	immediateContext->RSSetViewports(1, &viewportParticle);

	immediateContext->GSSetShader(gShaderParticle, nullptr, 0);
	immediateContext->PSSetShader(pShaderParticle, nullptr, 0);

	XMStoreFloat3(&getDirection.forwardVec, camera[0].GetForwardVector());
	XMStoreFloat3(&getDirection.upVec, camera[0].GetUpVector());;

	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(directionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	std::memcpy(subData.pData, &getDirection, sizeof(getDirection));
	immediateContext->Unmap(directionBuffer, 0);

	immediateContext->GSSetConstantBuffers(0, 1, &directionBuffer);
	camera[0].sendViewProjectionGS(camera[0], 1);
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

	std::vector<Mesh> mesh;

	vector<Camera> camera;
	for (int i = 0; i < 6; ++i)
		camera.push_back(Camera());

	std::vector<XMFLOAT3> particels;
	int playerPerspectiv = 0;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsView, viewport))
		return -1;

	if (!SetupShadowHelper(device, immediateContext, viewportShadow, WIDTH, HEIGHT, dsViewShadow, SRVShadow,
		gBufferRTV, gBufferSRV, swapChain, UAView))
		return -1;

	if (!SetupParticleHelper(device, immediateContext, UAViewP, swapChain, particleBuffer, particels, particlePosition,
		WIDTH, HEIGHT, dsViewParticle, viewportParticle, gBufferRTVParticle, gBufferSRVParticle))
		return -1;

	if (!SetupCubeMapping(device, WIDTH, HEIGHT, srvCubeMapping, UAVCubeMapping, viewportCubeMapping))
		return -1;

	if (!SetupShadowShaders(device, vShaderDepth, inputLayoutVSDepth))
		return -1;

	if (!SetupCubeMappingShaders(device, vShaderMainCubeMapping, inputLayoutVSMainCubeMapping, pShaderMainCubeMapping))
		return -1;

	if (!SetupRenderShaders(device, vShader, inputLayoutVS, hShader, dShader, pShader, cShader))
		return -1;

	if (!SetupParticleShaders(device, vShaderParticle, inputLayoutVSParticle, gShaderParticle, 
		pShaderParticle, cShaderParticle))
		return -1;

	if (!SetupSampleShaders(device, sampleState))
		return -1;

	if (!SetupSampleShadowShaders(device, sampleStateShadow))
		return -1;

	// Change to see Tesseleringen
	if (!SetupRasterizerState(device, rasterizerState))
		return -1;

	if (!SetupSampleStateCubeMapping(device, sampleStateCubeMapping))
		return -1;
	
	if (!SetupBuffers(device, lightBuffer, camBuffer, theWorldBuffer, directionBuffer,
		getDTTimeBuffer, lightData, camData, theWorld, getDirection, getDTTime, lightBufferTwo, lightBufferThree, lightBufferFour,
		lightDataTwo, lightDataThree, lightDataFour))
		return -1;

	if (!SetupModels(modelName, worldPos))
		return -1;

	// setup Camera:
	for (int i = 0; i < 6; ++i)
		camera[i].createConstantBuffer(camera[i], device, immediateContext);

	camera[1].AdjustPosition(0.0f, 4.299996f, -3.065905f);
	camera[2].AdjustPosition(-23.0f, 10.0f, 2.8f);
	camera[3].AdjustPosition(-0.91f, 10.0f, 27.4f);
	camera[4].AdjustPosition(33.6f, 10.0f, 3.1f);

	// light
	for (int i = 1; i < 5; ++i)
	{
		camera[i].SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
		camera[i].adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);
	}

	camera[5].adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);
	worldPos[0] = camera[5].GetPositionFloat3();

	vector<std::string> temp;
	for (int i = 0; i < modelName.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < temp.size(); j++)
		{
			if (temp[j] == modelName[i])
			{
				mesh.push_back(mesh[j]);
				mesh[i].setWorldPos(worldPos[i]);
				mesh[i].setUniqueId(i);
				found = true;
			}
		}

		if (!found)
		{
			if (!objReader(modelName[i], mesh, device, immediateContext, worldPos[i], i))
				return -1;
			temp.push_back(modelName[i]);
		}
	}

	temp.clear();
	modelName.clear();

	QuadTree* quadTree;
	quadTree = new QuadTree();

	quadTree->SetupQuadTree(mesh);


	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto start = std::chrono::system_clock::now();
		clearBig();
		std::vector<int> frosumRender;
		moveAbility(playerPerspectiv, camera);

		for (int i = 0; i < 4; i++)
		{
			ShadowPrePass(camera,  i);
			drawPrePass(immediateContext, mesh);
		}

		if (playerPerspectiv == 0)
		{
			for (int i = 0; i < 6; i++)
			{
				smallClear();
				RenderPreCube(camera, i);
				drawPreCube(mesh, quadTree->AllInViewFrustom(camera[0].sendFrustom(camera[0])));
				RenderComputerShader(camera);
			}
			smallClear();
		}

		Render(camera, playerPerspectiv);
		draw(mesh, quadTree->AllInViewFrustom(camera[0].sendFrustom(camera[0])));
		RenderComputerShader(camera);

		
		if (playerPerspectiv == 0)
		{
			particleSystem(camera);
			drawParticle(immediateContext, particels, worldPos, theWorld, theWorldBuffer, particleBuffer, dsView);
			RenderComputerShaderParticle(immediateContext, cShaderParticle, gBufferRTVParticle, gBufferSRVParticle, UAViewP,
				particels, getDTTimeBuffer, getDTTime);
			cubeMappingSystem(camera);
			drawCubeMapping(mesh, quadTree->AllInViewFrustom(camera[0].sendFrustom(camera[0])));
		}
		swapChain->Present(0, 0);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		dt = elapsed_seconds.count();
	}

	particels.clear();

	device->Release();
	immediateContext->Release();
	swapChain->Release();
	rtv->Release();

	UAView->Release();
	UAViewP->Release();

	dsView->Release();
	for (int i = 0; i < 4; i++)
		dsViewShadow[i]->Release();

	dsViewParticle->Release();
	for (int i = 0; i < 4; i++)
		SRVShadow[i]->Release();

	for (int i = 0; i < 6; i++)
		gBufferRTV[i]->Release();
	
	gBufferRTVParticle->Release();
	for (int i = 0; i < 6; i++)
		gBufferSRV[i]->Release();

	gBufferSRVParticle->Release();

	vShader->Release();
	vShaderDepth->Release();
	vShaderParticle->Release();
	vShaderMainCubeMapping->Release();
	gShaderParticle->Release();
	pShader->Release();
	pShaderParticle->Release();
	pShaderMainCubeMapping->Release();
	cShader->Release();
	cShaderParticle->Release();

	inputLayoutVS->Release();
	inputLayoutVSDepth->Release();
	inputLayoutVSParticle->Release();
	inputLayoutVSMainCubeMapping->Release();

	sampleState->Release();
	sampleStateShadow->Release();
	sampleStateCubeMapping->Release();

	lightBuffer->Release();
	lightBufferTwo->Release();
	lightBufferThree->Release();
	lightBufferFour->Release();

	camBuffer->Release();
	theWorldBuffer->Release();
	particleBuffer->Release();
	directionBuffer->Release();
	getDTTimeBuffer->Release();

	hShader->Release();
	dShader->Release();
	rasterizerState->Release();

	srvCubeMapping->Release();
	for (int i = 0; i < 6; i++)
		UAVCubeMapping[i]->Release();

	worldPos.clear();
	
	for (int i = 0; i < mesh.size(); i++)
		mesh[i].release();

	quadTree->release();
	delete quadTree;

	return 0;
}
