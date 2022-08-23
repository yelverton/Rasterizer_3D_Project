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
    ID3D11Buffer* shinessBuffer;
    XMVECTOR smallest;
    XMVECTOR biggest;
    std::vector<UINT> next;
    std::vector<UINT> size;
    TheWorld theWorld;
    std::vector<float> floatShiness;
    Shineness shineness;
    int unique;

protected:
    HRESULT CreateIndexBuffer(std::vector<DWORD> indexTriangle);
    HRESULT CreateVertexBuffer(std::vector<SimpleVertex> vertexTriangle);
    HRESULT SetupWorldMatrixs(XMFLOAT3 world);
    HRESULT SetupShinessBuffer();
    void CreateBoundingBox(XMFLOAT3 world);
    void SetContantBuffer();
    void SetShinessBuffer(int index);
    void changeWorld(XMFLOAT3 world);

public:
    Mesh(ID3D11Device* device, ID3D11DeviceContext* immediateContext, std::vector<SimpleVertex> vertexTriangle, 
        std::vector<DWORD> indexTriangle, std::vector<UINT> next, std::vector<UINT> size,
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ambient, 
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> diffuse,
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> specular,
        XMFLOAT3 world, int unique, XMVECTOR smallest, XMVECTOR biggest, std::vector<float> specularExponent);
    
    void release();
    void Draw();
    void DrawPrePass();
    void setWorldPos(XMFLOAT3 world);
    int NrOfSubMashes();
    void setUniqueId(int unique);
    int getUniqueId();
    DirectX::BoundingBox getBoundingBox();
    Mesh(const Mesh& mesh);
};