#include "QuadTree.h"
#include "Helper\ErrorLog.h"

bool QuadTree::SetupQuadTree(std::vector<Mesh> mesh)
{
	float xz = 50.0f;
	lastDepth = 5;

	SetupQuadTreeEnpty(rootNode = new Node(), 0, xz, 0, 0);
	ColliedWithQuadBox(rootNode, 0, mesh);

	return true;
}

void QuadTree::release()
{
	deleteQuadTree(rootNode, 0);
	delete rootNode;
}

void QuadTree::deleteQuadTree(Node* node, int depth)
{
	if (depth < lastDepth)
	{
		deleteQuadTree(node->upLeft, depth + 1);
		delete node->upLeft;
		deleteQuadTree(node->upRight, depth + 1);
		delete node->upRight;
		deleteQuadTree(node->downLeft, depth + 1);
		delete node->downLeft;
		deleteQuadTree(node->downRight, depth + 1);
		delete node->downRight;
	}
}


void QuadTree::SetupQuadTreeEnpty(Node* node, int depth, float xz, float xCenter, float zCenter)
{	
	xz /= 2;
	XMVECTOR smallest = XMVectorSet(-(xz) + xCenter, -30.f, -(xz) + zCenter, 0.0f);
	XMVECTOR biggest = XMVectorSet(xz + xCenter, 30.0f, xz + zCenter, 0.0f);

	DirectX::BoundingBox::CreateFromPoints(node->box, smallest, biggest);

	if (depth < lastDepth)
	{
		SetupQuadTreeEnpty(node->upLeft = new Node(), depth + 1, xz, xCenter - xz, zCenter + xz);
		SetupQuadTreeEnpty(node->upRight = new Node(), depth + 1, xz, xCenter + xz, zCenter + xz);
		SetupQuadTreeEnpty(node->downLeft = new Node(), depth + 1, xz, xCenter - xz, zCenter - xz);
		SetupQuadTreeEnpty(node->downRight = new Node(), depth + 1, xz, xCenter + xz, zCenter - xz);
	}
}

void QuadTree::ColliedWithQuadBox(Node* node, int depth, std::vector<Mesh> mesh)
{
	for (int i = 0; i < mesh.size(); i++)
	{
		if (mesh[i].getBoundingBox().Contains(node->box))
		{
			node->meshID.push_back(i);
		}
	}

	if (depth < lastDepth && node->meshID.size() > 0)
	{
		ColliedWithQuadBox(node->upLeft, depth + 1, mesh);
		ColliedWithQuadBox(node->upRight, depth + 1, mesh);
		ColliedWithQuadBox(node->downLeft, depth + 1, mesh);
		ColliedWithQuadBox(node->downRight, depth + 1, mesh);
	}
}


void QuadTree::ColliedWithViewFrustom(Node* node, int depth, DirectX::BoundingFrustum& frosum)
{
	if (node->meshID.size() > 0)
	{
		if (frosum.Contains(node->box) != 0)
		{
			if (depth < lastDepth)
			{
				ColliedWithViewFrustom(node->upLeft, depth + 1, frosum);
				ColliedWithViewFrustom(node->upRight, depth + 1, frosum);
				ColliedWithViewFrustom(node->downLeft, depth + 1, frosum);
				ColliedWithViewFrustom(node->downRight, depth + 1, frosum);
			}
			else
			{
				for (int i = 0; i < node->meshID.size(); i++)
				{
					inViewFrustom.push_back(node->meshID[i]);
				}
			}
		}
	}
}

std::vector<int> QuadTree::AllInViewFrustom(DirectX::BoundingFrustum frosum)
{
	inViewFrustom.clear();
	ColliedWithViewFrustom(rootNode, 0, frosum);
	std::vector<int> unique;
	for (int i = 0; i < inViewFrustom.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < unique.size(); j++) 
		{
			if (unique[j] == inViewFrustom[i])
				found = true;
		}

		if (!found)
			unique.push_back(inViewFrustom[i]);
	}


	return unique;
}



