#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <directxmath.h>
#include <directxcollision.h>
#include <vector>
using namespace DirectX;

#include "../Rasterizer_3D_Project/Helper/BufferType.h"
#include "Scene/Mesh.h"

struct NodePoint
{
	DirectX::BoundingBox box;
	std::vector<int> meshID;
};

struct Node
{
	XMFLOAT2 boundingBoxCenter;
	XMFLOAT2 boundingBoxSize;
	int depth;
	NodePoint* nodePoint = nullptr;
	Node* upLeft = nullptr;
	Node* upRight = nullptr;
	Node* downLeft = nullptr;
	Node* downRight = nullptr;
};

class QuadTree
{
private: 
	Node* rootNode;
	std::vector<int> inViewFrustom;

protected:
	void SetupQuadTreeEnpty(Node* node, int depth, float x, float z);
	void SetupBoundingBox(Node* node);
	void ColliedWithQuadBox(Node* node, int depth, std::vector<Mesh> mesh);
	void ColliedWithViewFrustom(Node* node, int depth, DirectX::BoundingFrustum& frosum);
public:
	bool SetupQuadTree(std::vector<Mesh> mesh);
	void release();
	std::vector<int> AllInViewFrustom(DirectX::BoundingFrustum frosum);
};