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
    XMFLOAT3 lightDirection;
    float padding;
};

struct LightDataTwo
{
    XMFLOAT3 posTwo;
    float ranageTwo; 
    XMFLOAT3 dirTwo;
    float coneTwo;
    XMFLOAT3 cSpotTwo;
    float paddingTwo;
    XMFLOAT3 attTwo;
    float paddingTwoTwo;
};

struct LightDataThree
{
    XMFLOAT3 posThree;
    float ranageThree;
    XMFLOAT3 dirThree;
    float coneThree;
    XMFLOAT3 cSpotThree;
    float paddingThree;
    XMFLOAT3 attThree;
    float paddingThreeThree;
};

struct LightDataFour
{
    XMFLOAT3 posFour;
    float ranageFour;
    XMFLOAT3 dirFour;
    float coneFour;
    XMFLOAT3 cSpotFour;
    float paddingFour;
    XMFLOAT3 attFour;
    float paddingFourFour;
};

struct CamData
{
    XMFLOAT3 cameraPosition;
    float padding;
};