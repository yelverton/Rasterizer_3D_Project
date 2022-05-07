#include "Setup/WindowHelper.h"
#include "Setup/D3D11Helper.h"
#include "Setup/PipelineHelper.h"
#include "Helper/BufferHelper.h"
#include "Scene/modelHelper.h"
#include "Scene/ModelReader/ObjHelper.h"

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
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewport;

	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* inputLayout;
	ID3D11SamplerState* sampleState;

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* camBuffer;
	struct LightData lightData;
	struct CamData camData;
	
	std::vector<std::string> modelName;
	std::vector<Mesh> mesh;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
		return -1;

	if (!SetupPipeline(device, vShader, pShader, inputLayout, sampleState))
		return -1;

	if (!SetupBuffers(device, lightBuffer, camBuffer, lightData, camData))
		return -1;

	if (!SetupModels(modelName))
		return -1;
		
	for (int i = 0; i < modelName.size(); i++)
		if (!objReader(modelName[i], mesh, device, immediateContext))
			return -1;

	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
