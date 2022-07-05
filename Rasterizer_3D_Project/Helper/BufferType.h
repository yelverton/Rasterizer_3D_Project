#pragma once
#include <directxmath.h>
using namespace DirectX;

struct TheWorld
{
    XMFLOAT4X4 worldMatrix;
};

struct VPmatrix
{
    XMFLOAT4X4 viewProj;
};

struct DepthBufferData
{
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};

struct BufferData
{
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};

struct LightData
{
    XMFLOAT3 lightPosition;
    float shininess;
};

struct CamData
{
    XMFLOAT3 cameraPosition;
    float padding;
};