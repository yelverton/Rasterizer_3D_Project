#include <chrono>
#include <ctime>

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

float dt = 0;

void clearBig(ID3D11DeviceContext*& immediateContext, ID3D11DepthStencilView* dsViewShadow[],
	ID3D11RenderTargetView* gBufferRTV[], ID3D11DepthStencilView*& dsView, ID3D11DepthStencilView*& dsViewParticle,
	ID3D11RenderTargetView* rtv, ID3D11RenderTargetView* gBufferRTVPreCube[])
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

void smallClear(ID3D11DeviceContext*& immediateContext, ID3D11DepthStencilView*& dsView,
	ID3D11RenderTargetView* gBufferRTV[])
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


void moveAbility(int& playerPerspectiv, Camera& camera, Camera& lightCamera, Camera& cubeMappingCamera,
	vector<XMFLOAT3>& worldPos, Camera& lightCameraTwo, Camera& lightCameraThree, Camera& lightCameraFour)
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

	if (playerPerspectiv == 0) {
		camera.moveCamera(camera, dt);
	} 
	else if (playerPerspectiv == 1) {
		lightCamera.moveCamera(lightCamera, dt);
		OutputDebugString(L"0&&x: ");
		OutputDebugString(std::to_wstring(lightCamera.GetPositionFloat3().x).c_str());
		OutputDebugString(L" y: ");
		OutputDebugString(std::to_wstring(lightCamera.GetPositionFloat3().y).c_str());
		OutputDebugString(L" z: ");
		OutputDebugString(std::to_wstring(lightCamera.GetPositionFloat3().z).c_str());
		OutputDebugString(L"\n");
	}
	else if (playerPerspectiv == 2) {
		lightCameraTwo.moveCamera(lightCameraTwo, dt);
		OutputDebugString(L"1&&x: ");
		OutputDebugString(std::to_wstring(lightCameraTwo.GetPositionFloat3().x).c_str());
		OutputDebugString(L" y: ");
		OutputDebugString(std::to_wstring(lightCameraTwo.GetPositionFloat3().y).c_str());
		OutputDebugString(L" z: ");
		OutputDebugString(std::to_wstring(lightCameraTwo.GetPositionFloat3().z).c_str());
		OutputDebugString(L"\n");
	}
	else if (playerPerspectiv == 3) {
		lightCameraThree.moveCamera(lightCameraThree, dt);
		OutputDebugString(L"2&&x: ");
		OutputDebugString(std::to_wstring(lightCameraThree.GetPositionFloat3().x).c_str());
		OutputDebugString(L" y: ");
		OutputDebugString(std::to_wstring(lightCameraThree.GetPositionFloat3().y).c_str());
		OutputDebugString(L" z: ");
		OutputDebugString(std::to_wstring(lightCameraThree.GetPositionFloat3().z).c_str());
		OutputDebugString(L"\n");
	}
	else if (playerPerspectiv == 4) {
		lightCameraFour.moveCamera(lightCameraFour, dt);
		OutputDebugString(L"3&&x: ");
		OutputDebugString(std::to_wstring(lightCameraFour.GetPositionFloat3().x).c_str());
		OutputDebugString(L" y: ");
		OutputDebugString(std::to_wstring(lightCameraFour.GetPositionFloat3().y).c_str());
		OutputDebugString(L" z: ");
		OutputDebugString(std::to_wstring(lightCameraFour.GetPositionFloat3().z).c_str());
		OutputDebugString(L"\n");
	}
} 

void ShadowPrePass(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView*& dsViewShadow, D3D11_VIEWPORT viewportShadow, Camera lightCamera,
	ID3D11VertexShader*& vShaderDepth, ID3D11InputLayout* inputLayoutShadow, Camera lightCameraTwo, Camera lightCameraThree, Camera lightCameraFour, int index)
{
	immediateContext->IASetInputLayout(inputLayoutShadow);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShaderDepth, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewportShadow);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
	immediateContext->OMSetRenderTargets(0, nullptr, dsViewShadow);

	if (index == 0)
		lightCamera.sendViewProjection(lightCamera, 1);
	else if (index == 1)
		lightCameraTwo.sendViewProjection(lightCameraTwo, 1);
	else if (index == 2)
		lightCameraThree.sendViewProjection(lightCameraThree, 1);
	else if (index == 3)
		lightCameraFour.sendViewProjection(lightCameraFour, 1);
}

void drawPrePass(ID3D11DeviceContext* immediateContext, vector<Mesh> mesh)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();
	for (int i = 0; i < mesh.size(); i++)
	{
		mesh[i].Draw();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
}

void RenderPreCube(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT viewport,
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampleState,
	ID3D11Buffer* camBuffer, struct CamData camData, Camera camera, ID3D11RenderTargetView* gBufferRTV[], Camera lightCamera, 
	ID3D11ShaderResourceView* SRVShadow[], ID3D11SamplerState* sampleStateShadow, int index, Camera lightCameraTwo, Camera lightCameraThree,
	Camera lightCameraFour)
{
	setCubeMappingRotation(camera, index);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);

	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(6, gBufferRTV, dsView);
	immediateContext->PSSetSamplers(0, 1, &sampleState);
	immediateContext->PSSetSamplers(1, 1, &sampleStateShadow);

	camera.sendViewProjection(camera, 1);
	lightCamera.sendViewProjection(lightCamera, 2);
	lightCameraTwo.sendViewProjection(lightCameraTwo, 3);
	lightCameraThree.sendViewProjection(lightCameraThree, 4);
	lightCameraFour.sendViewProjection(lightCameraFour, 5);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow[0]);
	immediateContext->PSSetShaderResources(4, 1, &SRVShadow[1]);
	immediateContext->PSSetShaderResources(5, 1, &SRVShadow[2]);
	immediateContext->PSSetShaderResources(6, 1, &SRVShadow[3]);
}

void Render(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT viewport, 
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampleState, 
	ID3D11Buffer* lightBuffer, ID3D11Buffer* camBuffer, struct LightData lightData, struct CamData camData, Camera camera,
	ID3D11RenderTargetView* gBufferRTV[], int playerPerspectiv, Camera lightCamera, ID3D11ShaderResourceView* SRVShadow[],
	ID3D11SamplerState* sampleStateShadow, ID3D11HullShader* hShader, ID3D11DomainShader* dShader,
	ID3D11RasterizerState* rasterizerState, Camera cubeMappingCamera, Camera lightCameraTwo, Camera lightCameraThree,
	Camera lightCameraFour)
{
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);

	camData.cameraPosition = camera.GetPositionFloat3();

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
		camera.sendViewProjection(camera, 1);
	else if (playerPerspectiv == 1)
		lightCamera.sendViewProjection(lightCamera, 1);
	else if (playerPerspectiv == 2)
		lightCameraTwo.sendViewProjection(lightCameraTwo, 1);
	else if (playerPerspectiv == 3)
		lightCameraThree.sendViewProjection(lightCameraThree, 1);
	else if (playerPerspectiv == 4)
		lightCameraFour.sendViewProjection(lightCameraFour, 1);



	lightCamera.sendViewProjection(lightCamera, 2);
	lightCameraTwo.sendViewProjection(lightCameraTwo, 3);
	lightCameraThree.sendViewProjection(lightCameraThree, 4);
	lightCameraFour.sendViewProjection(lightCameraFour, 5);

	immediateContext->PSSetShaderResources(3, 1, &SRVShadow[0]);
	immediateContext->PSSetShaderResources(4, 1, &SRVShadow[1]);
	immediateContext->PSSetShaderResources(5, 1, &SRVShadow[2]);
	immediateContext->PSSetShaderResources(6, 1, &SRVShadow[3]);

}

void draw(ID3D11DeviceContext* immediateContext, vector<Mesh>& mesh, std::vector<int> viewFrustom, Camera camera)
{
	for (int i = 0; i < viewFrustom.size(); i++)
	{
		if (viewFrustom[i] != 0)
			mesh[viewFrustom[i]].Draw();
	}

	//for (int i = 1; i < mesh.size(); i++)
	//	mesh[i].Draw();

	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->HSSetShader(nullptr, nullptr, 0);
	immediateContext->DSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
}

void drawPreCube(ID3D11DeviceContext* immediateContext, vector<Mesh>& mesh)
{
	for (int i = 1; i < mesh.size(); i++)
	{
		mesh[i].Draw();
	}

	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
}

void RenderComputerShader(ID3D11DeviceContext*& immediateContext, ID3D11ComputeShader* cShader,
	ID3D11UnorderedAccessView* UAView, ID3D11ShaderResourceView* gBufferSRV[], CamData& camData, 
	Camera camera, LightData& lightData, Camera lightCamera, ID3D11Buffer*& lightBuffer, ID3D11Buffer*& camBuffer,
	Camera lightCameraTwo, Camera lightCameraThree, Camera lightCameraFour, LightDataTwo& lightDataTwo, LightDataThree& lightDataThree,
	LightDataFour& lightDataFour, ID3D11Buffer*& lightBufferTwo, ID3D11Buffer*& lightBufferThree, ID3D11Buffer*& lightBufferFour)
{
	ID3D11RenderTargetView* nullRTV[6] = { nullptr };
	immediateContext->OMSetRenderTargets(6, nullRTV, nullptr);

	immediateContext->CSSetShader(cShader, nullptr, 0);
	immediateContext->CSSetShaderResources(0, 6, gBufferSRV);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &UAView, nullptr);

	camData.cameraPosition = camera.GetPositionFloat3();
	lightData.lightDirection = lightCamera.GetPositionFloat3();

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
	lightDataTwo.posTwo = lightCameraTwo.GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subLightTwo = {};
	immediateContext->Map(lightBufferTwo, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightTwo);
	std::memcpy(subLightTwo.pData, &lightDataTwo, sizeof(LightDataTwo));
	immediateContext->Unmap(lightBufferTwo, 0);
	immediateContext->CSSetConstantBuffers(2, 1, &lightBufferTwo);

	lightDataThree.posThree = lightCameraThree.GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subLightThree = {};
	immediateContext->Map(lightBufferThree, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightThree);
	std::memcpy(subLightThree.pData, &lightDataThree, sizeof(LightDataThree));
	immediateContext->Unmap(lightBufferThree, 0);
	immediateContext->CSSetConstantBuffers(3, 1, &lightBufferThree);

	lightDataFour.posFour = lightCameraFour.GetPositionFloat3();

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

//void RenderComputerShaderCube(ID3D11DeviceContext*& immediateContext, ID3D11ComputeShader* cShader,
//	ID3D11UnorderedAccessView* UAView, ID3D11ShaderResourceView* gBufferSRV[], CamData camData,
//	Camera camera, LightData lightData, Camera lightCamera, ID3D11Buffer* lightBuffer, ID3D11Buffer* camBuffer,
//	Camera lightCameraTwo, Camera lightCameraThree, Camera lightCameraFour, LightDataTwo lightDataTwo, LightDataThree lightDataThree,
//	LightDataFour lightDataFour, ID3D11Buffer* lightBufferTwo, ID3D11Buffer* lightBufferThree, ID3D11Buffer* lightBufferFour)
//{
//	ID3D11RenderTargetView* nullRTV[6] = { nullptr };
//	immediateContext->OMSetRenderTargets(6, nullRTV, nullptr);
//
//	immediateContext->CSSetShader(cShader, nullptr, 0);
//	immediateContext->CSSetShaderResources(0, 6, gBufferSRV);
//	immediateContext->CSSetUnorderedAccessViews(0, 1, &UAView, nullptr);
//
//	camData.cameraPosition = camera.GetPositionFloat3();
//
//	D3D11_MAPPED_SUBRESOURCE subLight = {};
//	immediateContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLight);
//	std::memcpy(subLight.pData, &lightData, sizeof(LightData));
//	immediateContext->Unmap(lightBuffer, 0);
//	immediateContext->CSSetConstantBuffers(0, 1, &lightBuffer);
//
//	D3D11_MAPPED_SUBRESOURCE subCam = {};
//	immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
//	std::memcpy(subCam.pData, &camData, sizeof(CamData));
//	immediateContext->Unmap(camBuffer, 0);
//	immediateContext->CSSetConstantBuffers(1, 1, &camBuffer);
//
//	// other lights
//	lightDataTwo.dirTwo = XMFLOAT3(5.0f, 5.0f, 0.0f);
//
//	D3D11_MAPPED_SUBRESOURCE subLightTwo = {};
//	immediateContext->Map(lightBufferTwo, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightTwo);
//	std::memcpy(subLight.pData, &lightDataTwo, sizeof(LightDataTwo));
//	immediateContext->Unmap(lightBufferTwo, 0);
//	immediateContext->CSSetConstantBuffers(2, 1, &lightBufferTwo);
//
//	lightDataThree.dirThree = lightCameraThree.GetPositionFloat3();
//
//	D3D11_MAPPED_SUBRESOURCE subLightThree = {};
//	immediateContext->Map(lightBufferThree, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightThree);
//	std::memcpy(subLight.pData, &lightDataThree, sizeof(LightDataThree));
//	immediateContext->Unmap(lightBufferThree, 0);
//	immediateContext->CSSetConstantBuffers(3, 1, &lightBufferThree);
//
//	lightDataFour.dirFour = lightCameraFour.GetPositionFloat3();
//
//	D3D11_MAPPED_SUBRESOURCE subLightFour = {};
//	immediateContext->Map(lightBufferFour, 0, D3D11_MAP_WRITE_DISCARD, 0, &subLightFour);
//	std::memcpy(subLight.pData, &lightDataFour, sizeof(LightDataFour));
//	immediateContext->Unmap(lightBufferFour, 0);
//	immediateContext->CSSetConstantBuffers(4, 1, &lightBufferFour);
//
//	immediateContext->Dispatch(32, 32, 1);
//
//	ID3D11UnorderedAccessView* NullUAV = nullptr;
//	immediateContext->CSSetUnorderedAccessViews(0, 1, &NullUAV, nullptr);
//	ID3D11ShaderResourceView* gBufferSRVNULL[6] = { nullptr };
//	immediateContext->CSSetShaderResources(0, 6, gBufferSRVNULL);
//
//	immediateContext->CSSetShader(nullptr, nullptr, 0);
//}

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

void cubeMappingSystem(ID3D11DeviceContext* immediateContext, ID3D11InputLayout* inputLayoutCubeMapping,
	ID3D11VertexShader* vShaderCubeMapping, D3D11_VIEWPORT viewportCubeMapping,
	ID3D11PixelShader* pShaderCubeMapping, ID3D11SamplerState* sampleCubaMapping,
	ID3D11ShaderResourceView* srvCubeMapping, ID3D11DepthStencilView* dsView, Camera cameraCubeMapping, CamData camData,
	ID3D11Buffer* camBuffer, ID3D11RenderTargetView* rtv)
{
	immediateContext->IASetInputLayout(inputLayoutCubeMapping);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShaderCubeMapping, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewportCubeMapping);
	immediateContext->PSSetShader(pShaderCubeMapping, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampleCubaMapping);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);

	cameraCubeMapping.sendViewProjection(cameraCubeMapping, 1);

	//ID3D11ShaderResourceView* no[3] = {nullptr};
	//immediateContext->PSSetShaderResources(0, 3, no);

	immediateContext->PSSetShaderResources(0, 1, &srvCubeMapping);

	camData.cameraPosition = cameraCubeMapping.GetPositionFloat3();

	D3D11_MAPPED_SUBRESOURCE subCam = {};
	immediateContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subCam);
	std::memcpy(subCam.pData, &camData, sizeof(CamData));
	immediateContext->Unmap(camBuffer, 0);
	immediateContext->PSSetConstantBuffers(0, 1, &camBuffer);
}

void drawCubeMapping(ID3D11DeviceContext* immediateContext, Camera cubeMappingCamera, vector<Mesh> mesh,
	vector<XMFLOAT3> worldPos, struct TheWorld theWorld, ID3D11Buffer* theWorldBuffer)
{
	DirectX::XMMATRIX Identity = XMMatrixIdentity();
	Identity = XMMatrixTranslation(worldPos[0].x, worldPos[0].y, worldPos[0].z);
	XMStoreFloat4x4(&theWorld.worldMatrix, XMMatrixTranspose(Identity));

	D3D11_MAPPED_SUBRESOURCE subData = {};
	immediateContext->Map(theWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	std::memcpy(subData.pData, &theWorld, sizeof(TheWorld));
	immediateContext->Unmap(theWorldBuffer, 0);
	immediateContext->VSSetConstantBuffers(0, 1, &theWorldBuffer);

	mesh[0].DrawPrePass();

	immediateContext->VSSetShader(nullptr, nullptr, 0);
	immediateContext->PSSetShader(nullptr, nullptr, 0);
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
	ID3D11UnorderedAccessView* UAViewC;

	ID3D11DepthStencilView* dsView;
	ID3D11DepthStencilView* dsViewShadow[4];
	ID3D11DepthStencilView* dsViewParticle;
	ID3D11DepthStencilView* dsViewCubeMapping;
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
	ID3D11VertexShader* vShaderPreCubeMapping;
	ID3D11VertexShader* vShaderMainCubeMapping;
	ID3D11GeometryShader* gShaderParticle;
	ID3D11PixelShader* pShader;
	ID3D11PixelShader* pShaderParticle;
	ID3D11PixelShader* pShaderPreCubeMapping;
	ID3D11PixelShader* pShaderMainCubeMapping;
	ID3D11ComputeShader* cShader;
	ID3D11ComputeShader* cShaderParticle;
	ID3D11ComputeShader* cShaderPreCube;

	ID3D11InputLayout* inputLayoutVS;
	ID3D11InputLayout* inputLayoutVSDepth;
	ID3D11InputLayout* inputLayoutVSParticle;
	ID3D11InputLayout* inputLayoutVSPreCubeMapping;
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
	ID3D11ShaderResourceView* gBufferSRVPreCube[3];

	std::vector<std::string> modelName;
	std::vector<XMFLOAT3> worldPos;

	std::vector<Mesh> mesh;
	Camera camera;

	// light
	Camera lightCamera;
	Camera lightCameraTwo;
	Camera lightCameraThree;
	Camera lightCameraFour;
	
	Camera cubeMappingCamera;

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

	if (!SetupCubeMapping(device, WIDTH, HEIGHT, srvCubeMapping, UAVCubeMapping, viewportCubeMapping, dsViewCubeMapping,
		gBufferRTVPreCube, gBufferSRVPreCube))
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
	camera.createConstantBuffer(camera, device, immediateContext);

	// Light
	lightCamera.createConstantBuffer(lightCamera, device, immediateContext);
	lightCameraTwo.createConstantBuffer(lightCameraTwo, device, immediateContext);
	lightCameraThree.createConstantBuffer(lightCameraThree, device, immediateContext);
	lightCameraFour.createConstantBuffer(lightCameraFour, device, immediateContext);
	
	cubeMappingCamera.createConstantBuffer(cubeMappingCamera, device, immediateContext);

	lightCamera.AdjustPosition(0.0f, 4.299996f, -3.065905f);
	lightCameraTwo.AdjustPosition(-23.0f, 10.0f, 2.8f);
	lightCameraThree.AdjustPosition(-0.91f, 10.0f, 27.4f);
	lightCameraFour.AdjustPosition(33.6f, 10.0f, 3.1f);

	// light
	lightCamera.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
	lightCamera.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);
	
	lightCameraTwo.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
	lightCameraTwo.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);
	
	lightCameraThree.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
	lightCameraThree.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);

	lightCameraFour.SetLookAtPos(XMFLOAT3(0.0f, 1.0f, 0.0f));
	lightCameraFour.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);

	cubeMappingCamera.adjustProjectionMatrix(DirectX::XM_PI * 0.6, float(WIDTH / HEIGHT), 0.1, 1000.f);
	worldPos[0] = cubeMappingCamera.GetPositionFloat3();

	for (int i = 0; i < modelName.size(); i++)
		if (!objReader(modelName[i], mesh, device, immediateContext, worldPos[i], i))
			return -1;

	QuadTree quadTree;
	quadTree.SetupQuadTree(mesh);

	//std::vector<XMFLOAT4X4> worldFrustom;
	//for (int i = 0; i < worldPos.size(); i++)
	//{
	//	DirectX::XMMATRIX worldMatrix = XMMatrixIdentity();
	//	worldMatrix = XMMatrixTranslation(worldPos[i].x, worldPos[i].y, worldPos[i].z);
	//	worldFrustom.push_back(XMFLOAT4X4());
	//	XMStoreFloat4x4(&worldFrustom[i], XMMatrixTranspose(worldMatrix));
	//}

	//frustom.SetupFrustom(camera.GetProjection(), bigSmall, worldFrustom, mesh);

	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto start = std::chrono::system_clock::now();
		clearBig(immediateContext, dsViewShadow, gBufferRTV, dsView, dsViewParticle, rtv, gBufferRTVPreCube);
		std::vector<int> frosumRender;
		moveAbility(playerPerspectiv, camera, lightCamera, cubeMappingCamera, worldPos, lightCameraTwo, lightCameraThree, lightCameraFour);

		for (int i = 0; i < 4; i++)
		{
			ShadowPrePass(immediateContext, dsViewShadow[i], viewportShadow, lightCamera, vShaderDepth, inputLayoutVSDepth,
				lightCameraTwo, lightCameraThree, lightCameraFour, i);
			drawPrePass(immediateContext, mesh);
		}

		if (playerPerspectiv == 0)
		{
			for (int i = 0; i < 6; i++)
			{
				smallClear(immediateContext, dsView, gBufferRTV);
				RenderPreCube(immediateContext, dsView, viewport, vShader, pShader, inputLayoutVS, sampleState, camBuffer,
					camData, cubeMappingCamera, gBufferRTV, lightCamera, SRVShadow, sampleState, i, lightCameraTwo,
					lightCameraThree, lightCameraFour);
				drawPreCube(immediateContext, mesh);
				//RenderComputerShaderCube(immediateContext, cShader, UAVCubeMapping[i], gBufferSRV, camData, cubeMappingCamera, lightData,
				//	lightCamera, lightBuffer, camBuffer, lightCameraTwo, lightCameraThree, lightCameraFour, lightDataTwo, lightDataThree, 
				//	lightDataFour, lightBufferTwo, lightBufferThree, lightBufferFour);
				RenderComputerShader(immediateContext, cShader, UAVCubeMapping[i], gBufferSRV, camData, camera, lightData,
					lightCamera, lightBuffer, camBuffer, lightCameraTwo, lightCameraThree, lightCameraFour, lightDataTwo, lightDataThree,
					lightDataFour, lightBufferTwo, lightBufferThree, lightBufferFour);
			}
			smallClear(immediateContext, dsView, gBufferRTV);
		}

		Render(immediateContext, dsView, viewport, vShader, pShader, inputLayoutVS, sampleState, lightBuffer,
			camBuffer, lightData, camData, camera, gBufferRTV, playerPerspectiv, lightCamera, SRVShadow,
			sampleStateShadow, hShader, dShader, rasterizerState, cubeMappingCamera, lightCameraTwo, lightCameraThree,
			lightCameraFour);
		draw(immediateContext, mesh, quadTree.AllInViewFrustom(camera.sendFrustom(camera)), camera);
		RenderComputerShader(immediateContext, cShader, UAView, gBufferSRV, camData, camera, lightData,
			lightCamera, lightBuffer, camBuffer, lightCameraTwo, lightCameraThree, lightCameraFour, lightDataTwo, lightDataThree,
			lightDataFour, lightBufferTwo, lightBufferThree, lightBufferFour);

		
		if (playerPerspectiv == 0)
		{
			particleSystem(immediateContext, inputLayoutVSParticle, vShaderParticle, dsView, viewportParticle, gShaderParticle, pShaderParticle,
				cShaderParticle, directionBuffer, getDirection, camera, camBuffer, camData, rtv);
			drawParticle(immediateContext, particels, worldPos, theWorld, theWorldBuffer, particleBuffer, dsView);
			RenderComputerShaderParticle(immediateContext, cShaderParticle, gBufferRTVParticle, gBufferSRVParticle, UAViewP,
				particels, getDTTimeBuffer, getDTTime);
			cubeMappingSystem(immediateContext, inputLayoutVSMainCubeMapping, vShaderMainCubeMapping, viewportCubeMapping,
				pShaderMainCubeMapping, sampleStateCubeMapping, srvCubeMapping, dsView, camera, camData, camBuffer, rtv);
			drawCubeMapping(immediateContext, cubeMappingCamera, mesh, worldPos, theWorld, theWorldBuffer);

		}
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
	for (int i = 0; i < 4; i++)
	{
		dsViewShadow[i]->Release();
		SRVShadow[i]->Release();

	}
	inputLayoutVSParticle->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();
	hShader->Release();
	dShader->Release();
	/*resterizerState->Release();*/

	for (int i = 0; i < 6; i++)
	{
		gBufferRTV[i]->Release();
		gBufferSRV[i]->Release();
		//uavCubeMapping[i]->Release();
	}

	return 0;
}
