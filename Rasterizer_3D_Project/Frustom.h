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
	std::vector<Mesh> mesh;
	Node* upLeft = nullptr;
	Node* upRight = nullptr;
	Node* downLeft = nullptr;
	Node* downRight = nullptr;
};

class Frustom
{
private: 
	std::vector<BoundingBox> objects;
	Node* rootNode;
	bool exist = false;
	std::vector<Node*> nodeVec;
	std::vector<Node*> currentNode;
	std::vector<Mesh> currentMesh;
	std::vector<BoundingBox> taken;

protected:
	void addTree(Node* node, int depth);
	void AddcolliedWithBoundingBox(Node* node, int depth, std::vector<Mesh> mesh);
	void GetcolliedWithFrustom(Node* node, int depth, DirectX::BoundingFrustum frosum);
public:
	bool SetupFrustom(XMMATRIX projection, std::vector<BigSmall> bigSmall, std::vector<XMFLOAT4X4> worldFrustomm, std::vector<Mesh> mesh);
	std::vector<int> getFrustom(XMMATRIX viewProj);
};