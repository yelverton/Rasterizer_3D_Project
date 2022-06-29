#include "D3D11Helper.h"
#include "../Helper/ErrorLog.h"

bool CreateInterfaces(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, UINT width, UINT height, HWND window)
{
	UINT flags = 0;
	/*if (_DEBUG)
		flags = D3D11_CREATE_DEVICE_DEBUG;*/

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Default
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;



	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
		flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &immediateContext);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create InterFaces to device and swapChain!");
		return false;
	}

	return true;
}

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv)
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer for render target view!" << std::endl;
		return false;
	}

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	if (FAILED(device->CreateRenderTargetView(backBuffer, NULL, &rtv)))
	{
		ErrorLog::Log("Failed to create Render Target View!");
		return false;
	}

	backBuffer->Release();
	return true;

}

bool CreateUnorderedAccessView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11UnorderedAccessView*& UAView)
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

bool CreateGBuffers(ID3D11Device* device, UINT width, UINT height, 
	ID3D11RenderTargetView* gBufferRTV[6], ID3D11ShaderResourceView* gBufferSRV[6])
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width = width;
	texDesc.Height = height;
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

bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11DepthStencilView*& dsView)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* dsTexture;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		ErrorLog::Log("Failed to create depth stencil texture!");
		return false;
	}

	if (FAILED(device->CreateDepthStencilView(dsTexture, 0, &dsView)))
	{
		ErrorLog::Log("Failed to create depth stencil View!");
		return false;
	}

	dsTexture->Release();
	return true;
}

bool CreateDepthStencilForShadow(ID3D11Device* device, UINT width, UINT height, ID3D11DepthStencilView*& dsViewShadow,
	ID3D11ShaderResourceView*& SRVShadow)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = (width * 2);
	textureDesc.Height = (width * 2);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* dsTexture;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		ErrorLog::Log("Failed to create depth stencil texture!");
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Flags = 0;

	if (FAILED(device->CreateDepthStencilView(dsTexture, &depthStencilViewDesc, &dsViewShadow)))
	{
		ErrorLog::Log("Failed to create depth stencil View for shadow!");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderDesc.Texture2D.MostDetailedMip = 0;
	shaderDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(dsTexture, &shaderDesc, &SRVShadow)))
	{
		ErrorLog::Log("Failed to create SRV!");
		return false;
	}

	dsTexture->Release();
	return true;
}

void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext,
	IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv, ID3D11UnorderedAccessView*& UAView,
	ID3D11DepthStencilView*& dsView, ID3D11DepthStencilView*& dsViewShadow, D3D11_VIEWPORT& viewport, 
	ID3D11ShaderResourceView*& SRVShadow, ID3D11RenderTargetView* gBufferRTV[6], ID3D11ShaderResourceView* gBufferSRV[6])
{
	if (!CreateInterfaces(device, immediateContext, swapChain, width, height, window))
		return false;

	if (!CreateRenderTargetView(device, swapChain, rtv))
		return false;
	
	if (!CreateDepthStencil(device, width, height, dsView))
		return false;

	// [Shadow Stage] Depth stencil
	if (!CreateDepthStencilForShadow(device, width, height, dsViewShadow, SRVShadow))
		return false;

	// Computer Shader:
	if (!CreateUnorderedAccessView(device, swapChain, UAView))
		return false;

	// Computer Shader:
	if (!CreateGBuffers(device, width, height, gBufferRTV, gBufferSRV))
		return false;

	SetViewport(viewport, width, height);

	return true;
}