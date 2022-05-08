#pragma once
#include <directxmath.h>
using namespace DirectX;

struct MatrixData
{
    XMMATRIX worldMatrix;
    XMMATRIX wvpMatrix;
};

struct BufferData
{
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};

struct VPmatrix
{
    XMFLOAT4X4 viewProj;
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