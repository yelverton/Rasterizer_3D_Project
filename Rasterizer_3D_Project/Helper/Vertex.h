#pragma once
#include <directxmath.h>
using namespace DirectX;

struct SimpleVertex
{
    XMFLOAT3 pos; // Position
    XMFLOAT2 uv; // UV coordination
    XMFLOAT3 nor; // Normal

    SimpleVertex() {
        pos = XMFLOAT3{ 0.0f,0.0f,0.0f };
        uv = XMFLOAT2{ 0.0f,0.0f };
        nor = XMFLOAT3{ 0.0f,0.0f,0.0f };
    };
    SimpleVertex(XMFLOAT3 pos, XMFLOAT2 uv, XMFLOAT3 nor) : pos(pos), uv(uv), nor(nor) {};
};