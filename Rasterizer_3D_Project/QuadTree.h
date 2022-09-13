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

struct Node
{
	DirectX::BoundingBox box;
	std::vector<int> meshID;
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
	int lastDepth;

protected:
	void SetupQuadTreeEnpty(Node* node, int depth, float xz, float xCenter, float zCenter);
	void ColliedWithQuadBox(Node* node, int depth, std::vector<Mesh> mesh);
	void ColliedWithViewFrustom(Node* node, int depth, DirectX::BoundingFrustum& frosum);
	void deleteQuadTree(Node* node, int depth);
public:
	bool SetupQuadTree(std::vector<Mesh> mesh);
	void release();
	std::vector<int> AllInViewFrustom(DirectX::BoundingFrustum frosum);
};