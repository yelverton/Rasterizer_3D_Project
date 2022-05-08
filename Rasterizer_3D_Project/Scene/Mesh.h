#pragma once
#include "../Helper/Vertex.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxmath.h>
#include <string>
using namespace DirectX;

class Mesh
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
    DWORD size = 0;

protected:
    HRESULT CreateIndexBuffer(std::vector<DWORD> indexTriangle);
    HRESULT CreateVertexBuffer(std::vector<SimpleVertex> vertexTriangle);
public:
    Mesh(ID3D11Device* device, ID3D11DeviceContext* immediateContext, std::vector<SimpleVertex> vertexTriangle, std::vector<DWORD> indexTriangle);
    void Draw();
    Mesh(const Mesh& mesh);
};