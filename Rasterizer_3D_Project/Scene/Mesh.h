#pragma once
#include "../Helper/Vertex.h"
#include "../Helper/BufferType.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxmath.h>
#include <string>
using namespace DirectX;
#include <directxcollision.h>

class Mesh
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ambient;
    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> diffuse;
    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> specular;
    DirectX::BoundingBox boundingBox;
    ID3D11Buffer* theWorldBuffer;
    std::vector<UINT> next;
    std::vector<UINT> size;
    TheWorld theWorld;
    int unique;

protected:
    HRESULT CreateIndexBuffer(std::vector<DWORD> indexTriangle);
    HRESULT CreateVertexBuffer(std::vector<SimpleVertex> vertexTriangle);
    HRESULT SetupWorldMatrixs(XMFLOAT3 world);
    void CreateBoundingBox(XMVECTOR smallest, XMVECTOR biggest, XMFLOAT3 world);
    void SetContantBuffer();
public:
    Mesh(ID3D11Device* device, ID3D11DeviceContext* immediateContext, std::vector<SimpleVertex> vertexTriangle, 
        std::vector<DWORD> indexTriangle, std::vector<UINT> next, std::vector<UINT> size,
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ambient, 
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> diffuse,
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> specular,
        XMFLOAT3 world, int unique, XMVECTOR smallest, XMVECTOR biggest);
    
    void Draw();
    void DrawPrePass();
    int NrOfSubMashes();
    void changeWorld(XMFLOAT3 world);
    int getUniqueId();
    DirectX::BoundingBox getBoundingBox();
    Mesh(const Mesh& mesh);
};