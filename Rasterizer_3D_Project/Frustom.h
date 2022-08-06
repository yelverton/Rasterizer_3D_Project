#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <directxmath.h>
#include <directxcollision.h>
#include <vector>
using namespace DirectX;

#include "../Rasterizer_3D_Project/Helper/BufferType.h"


class Frustom
{
private: 
	DirectX::BoundingFrustum frosum;
	std::vector<BoundingBox> objects;

protected:
	void goThrowFrustom(float xAnswer, float zAnswer);

public:
	bool SetupFrustom(XMMATRIX projection, std::vector<BigSmall> bigSmall);
	void goThrowFrustom();
};