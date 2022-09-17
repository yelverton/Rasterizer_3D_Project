#include "mainRender.h"
#include "../Helper/ErrorLog.h"

MainRender::MainRender(UINT w, UINT h)
	:WIDTH(w), HEIGHT(h)
{
	for (int i = 0; i < 6; ++i)
	{
		gBufferRTV[i] = nullptr;
		gBufferSRV[i] = nullptr;
	}
}

bool MainRender::Initilize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, 
	IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView)
{
	this->device = device;
	this->immediateContext = immediateContext;

	// [Computer shader] Unorderd access view:
	if (!CreateUnorderedAccessView(swapChain, UAView))
		return false;

	// [Computer shader] Gbuffers:
	if (!CreateGBuffers())
		return false;

	p = new PipelineHelper(device);
	p->VSet(vShader, inputLayout);
	p->HSet(hShader);
	p->DSet(dShader);
	p->RSet(rasterizerState);
	p->GSet(gShader);
	p->PSet(pShader);
	p->CSet(cShader);
	delete p;

	return true;
}

void MainRender::Draw()
{
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);

	camData.cameraPosition = camera->GetPositionFloat3();

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

	if (playerPerspectiv == 0)
		camera->sendViewProjection(camera, 1);
	else if (playerPerspectiv == 1)
		lightCamera->sendViewProjection(lightCamera, 1);
	else if (playerPerspectiv == 2)
		lightCameraTwo->sendViewProjection(lightCameraTwo, 1);
	else if (playerPerspectiv == 3)
		lightCameraThree->sendViewProjection(lightCameraThree, 1);
	else if (playerPerspectiv == 4)
		lightCameraFour->sendViewProjection(lightCameraFour, 1);

	lightCamera->sendViewProjection(lightCamera, 2);
	lightCameraTwo->sendViewProjection(lightCameraTwo, 3);
	lightCameraThree->sendViewProjection(lightCameraThree, 4);
	lightCameraFour->sendViewProjection(lightCameraFour, 5);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow[0]);
	immediateContext->PSSetShaderResources(4, 1, &SRVShadow[1]);
	immediateContext->PSSetShaderResources(5, 1, &SRVShadow[2]);
	immediateContext->PSSetShaderResources(6, 1, &SRVShadow[3]);
}

bool MainRender::CreateUnorderedAccessView(IDXGISwapChain*& swapChain, ID3D11UnorderedAccessView*& UAView)
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
		ErrorLog::Log("Failed to get back buffer for unorded access view!");
		return false;
	}

	// use the back buffer address to create the uordered access view
	// null as description to base it on the backbuffers values
	if (FAILED(device->CreateUnorderedAccessView(backBuffer, NULL, &UAView))) {
		ErrorLog::Log("Failed to unoarded access view!");
		return false;
	}

	backBuffer->Release();
	return true;
}

bool MainRender::CreateGBuffers()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width = WIDTH;
	texDesc.Height = HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* gBuffTex[6];

	for (int i = 0; i < 6; i++)
	{
		if (FAILED(device->CreateTexture2D(&texDesc, NULL, &gBuffTex[i])))
		{
			ErrorLog::Log("Failed to setup TEXTURE2D for computer shader in CreateGBuffer");
			return false;
		}

		if (FAILED(device->CreateRenderTargetView(gBuffTex[i], NULL, &gBufferRTV[i])))
		{
			ErrorLog::Log("Failed to setup RenderTargetView for computer shader in CreateGBuffer");
			return false;
		}

		if (FAILED(device->CreateShaderResourceView(gBuffTex[i], NULL, &gBufferSRV[i])))
		{
			ErrorLog::Log("Failed to setup ShaderResourceView for computer shader in CreateGBuffer");
			return false;
		}

		gBuffTex[i]->Release();
	}

	return true;
}


