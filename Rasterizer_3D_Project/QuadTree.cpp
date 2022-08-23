#include "QuadTree.h"
#include "Helper\ErrorLog.h"

void QuadTree::SetupBoundingBox(Node* node)
{
	node->nodePoint = new NodePoint();

	XMVECTOR smallest = XMVectorSet(-(node->boundingBoxSize.x) + node->boundingBoxCenter.x, -20.f, -(node->boundingBoxSize.y) + node->boundingBoxCenter.y, 0.0f);
	XMVECTOR biggest = XMVectorSet(node->boundingBoxSize.x + node->boundingBoxCenter.x, 30.0f, node->boundingBoxSize.y + node->boundingBoxCenter.y, 0.0f);

	DirectX::BoundingBox box;
	DirectX::BoundingBox::CreateFromPoints(box, smallest, biggest);
	node->nodePoint->box = box;
}

bool QuadTree::SetupQuadTree(std::vector<Mesh> mesh)
{
	float x = 50.0f;
	float z = 50.0f;

	rootNode = new Node();
	rootNode->boundingBoxCenter = XMFLOAT2(0.0f, 0.0f);

	SetupQuadTreeEnpty(rootNode, 0, x, z);

	ColliedWithQuadBox(rootNode, 0, mesh);

	return true;
}

void QuadTree::release()
{
	delete rootNode;
}


void QuadTree::SetupQuadTreeEnpty(Node* node, int depth, float x, float z)
{
	node->boundingBoxSize = XMFLOAT2(x, z);
	node->depth = depth;
	
	x *= 0.5f;
	z *= 0.5f;

	// Bootom left corner
	float xCenter = x;
	float zCenter = z;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0: // Top left
			{
				if (depth > 0)
				{
					SetupBoundingBox(node);
				}

				if (depth < 4)
				{
					node->upLeft = new Node();
					xCenter = node->boundingBoxCenter.x - x;
					zCenter = node->boundingBoxCenter.y + x;
					node->upLeft->boundingBoxCenter = XMFLOAT2(xCenter, zCenter);
					SetupQuadTreeEnpty(node->upLeft, depth + 1, x, z);
				}

				break;
			}
			case 1: // Top right
			{
				if (depth > 0)
				{
					SetupBoundingBox(node);
				}

				if (depth < 4)
				{
					node->upRight = new Node();
					xCenter = node->boundingBoxCenter.x + x;
					zCenter = node->boundingBoxCenter.y + z;
					node->upRight->boundingBoxCenter = XMFLOAT2(xCenter, zCenter);
					SetupQuadTreeEnpty(node->upRight, depth + 1, x, z);
				}

				break;
			}
			case 2: // bottom left
			{
				if (depth > 0)
				{
					SetupBoundingBox(node);
				}

				if (depth < 4)
				{
					node->downLeft = new Node();
					xCenter = node->boundingBoxCenter.x - x;
					zCenter = node->boundingBoxCenter.y - z;
					node->downLeft->boundingBoxCenter = XMFLOAT2(xCenter, zCenter);
					SetupQuadTreeEnpty(node->downLeft, depth + 1, x, z);
				}

				break;
			}
			case 3:
			{
				if (depth > 0)
				{
					SetupBoundingBox(node);
				}

				if (depth < 4)
				{
					node->downRight = new Node();
					xCenter = node->boundingBoxCenter.x + x;
					zCenter = node->boundingBoxCenter.y - z;
					node->downRight->boundingBoxCenter = XMFLOAT2(xCenter, zCenter);
					SetupQuadTreeEnpty(node->downRight, depth + 1, x, z);
				}

				break;
			}
		}
	}

}

void QuadTree::ColliedWithQuadBox(Node* node, int depth, std::vector<Mesh> mesh)
{

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				if (depth < 4)
				{
					ColliedWithQuadBox(node->upLeft, depth + 1, mesh);
				}

				break;
			}
			case 1:
			{
				if (depth < 4)
				{
					ColliedWithQuadBox(node->upRight, depth + 1, mesh);
				}

				break;
			}
			case 2:
			{
				if (depth < 4)
				{
					ColliedWithQuadBox(node->downLeft, depth + 1, mesh);
				}

				break;
			}
			case 3:
			{
				if (depth < 4)
				{
					ColliedWithQuadBox(node->downRight, depth + 1, mesh);
				}

				break;
			}
		}
	}

	if (node->nodePoint != nullptr && depth > 3)
	{
		for (int i = 0; i < mesh.size(); i++)
		{
			if (mesh[i].getBoundingBox().Contains(node->nodePoint->box) != 0)
			{
				node->nodePoint->meshID.push_back(mesh[i].getUniqueId());
			}
		}
	}
}


void QuadTree::ColliedWithViewFrustom(Node* node, int depth, DirectX::BoundingFrustum& frosum)
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				if (depth < 4)
				{
					ColliedWithViewFrustom(node->upLeft, depth + 1, frosum);
				}

				break;
			}
			case 1:
			{
				if (depth < 4)
				{
					ColliedWithViewFrustom(node->upRight, depth + 1, frosum);
				}
			}
			case 2:
			{
				if (depth < 4)
				{
					ColliedWithViewFrustom(node->downLeft, depth + 1, frosum);
				}

				break;
			}
			case 3:
			{
				if (depth < 4)
				{
					ColliedWithViewFrustom(node->downRight, depth + 1, frosum);
				}
				break;
			}
		}
	}

	if (node->nodePoint != nullptr && depth > 3)
	{
		if (frosum.Contains(node->nodePoint->box) != 0)
		{
			for (int i = 0; i < node->nodePoint->meshID.size(); i++)
			{
				inViewFrustom.push_back(node->nodePoint->meshID[i]);
			}
		}
	}

}

std::vector<int> QuadTree::AllInViewFrustom(DirectX::BoundingFrustum frosum)
{
	inViewFrustom.clear();
	ColliedWithViewFrustom(rootNode, 0, frosum);
	//rootNode->box.CreateFromPoints()
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


