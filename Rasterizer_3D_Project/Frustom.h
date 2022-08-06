#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <directxmath.h>
#include <directxcollision.h>
#include <vector>
using namespace DirectX;

#include "../Rasterizer_3D_Project/Helper/BufferType.h"

struct Node
{
	DirectX::BoundingBox box;
	Node* upLeft = nullptr;
	Node* upRight = nullptr;
	Node* downLeft = nullptr;
	Node* downRight = nullptr;
};

class Frustom
{
private: 
	DirectX::BoundingFrustum frosum;
	std::vector<BoundingBox> objects;
	Node* rootNode;
	std::vector<Node*> nodeVec;

protected:
	void goThrowFrustom(float xAnswer, float zAnswer);
	void addFrustom(Node* node, int depth, int topLeft, int topRight);
	void colliedWithBoundingBox(Node* node, int depth, std::vector<BoundingBox*> object);
public:
	bool SetupFrustom(XMMATRIX projection, std::vector<BigSmall> bigSmall, std::vector<XMFLOAT4X4> worldFrustom);
	void goThrowFrustom();
};