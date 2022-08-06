#pragma once
#include <directxmath.h>
using namespace DirectX;

struct TheWorld
{
    XMFLOAT4X4 worldMatrix;
};

struct GetDtTime
{
    XMFLOAT3 paddingDt;
    float dt;
};

struct GetDirection
{
    XMFLOAT3 forwardVec;
    float padding0;
    XMFLOAT3 upVec;
    float padding1;
};

struct ParticlePosition
{
    XMFLOAT3 pos;
    float direction;
};

struct VPmatrix
{
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
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

struct BigSmall
{
    XMVECTOR biggest;
    XMVECTOR smallest;
};