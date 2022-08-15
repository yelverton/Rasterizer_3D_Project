#include "QuadTree.h"
#include "Helper\ErrorLog.h"

void QuadTree::SetupBoundingBox(Node*& node)
{
	node->nodePoint = new NodePoint();
	//node->nodePoint->box.Center = XMFLOAT3((float)node->boundingBoxCenter.x, 0.0f, (float)node->boundingBoxCenter.y);
	//node->nodePoint->box.Extents = XMFLOAT3(node->boundingBoxCenter.x, 10.0f, float(node->boundingBoxCenter.y + node->boundingBoxSize.y));
	/*node->nodePoint->box.Extents = XMFLOAT3(node->boundingBoxSize.y, node->boundingBoxSize.y, node->boundingBoxSize.y);*/

	XMFLOAT3 world = XMFLOAT3(node->boundingBoxCenter.x, 0.0f, node->boundingBoxCenter.y);
	XMVECTOR smallest = XMVectorSet(-(node->boundingBoxSize.x), -20.f, -(node->boundingBoxSize.y), 0.0f);
	XMVECTOR biggest = XMVectorSet(node->boundingBoxSize.x, 30.0f, node->boundingBoxSize.y, 0.0f);

	DirectX::BoundingBox box;
	DirectX::BoundingBox::CreateFromPoints(box, smallest, biggest);
	DirectX::XMMATRIX tempScale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX tempRota = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX tempTrans = DirectX::XMMatrixTranslation(world.x, world.y, world.z);

	DirectX::XMMATRIX WMBB = XMMatrixIdentity();
	WMBB = DirectX::XMMatrixMultiply(tempScale, tempRota);
	WMBB = DirectX::XMMatrixMultiply(WMBB, tempTrans);

	box.Transform(box, WMBB);
	node->nodePoint->box = box;
}

bool QuadTree::SetupQuadTree(std::vector<Mesh> mesh)
{
	float x = 64.0f;
	float z = 64.0f;

	rootNode = new Node();
	rootNode->boundingBoxCenter = XMFLOAT2(0.0f, 0.0f);

	SetupQuadTreeEnpty(rootNode, 0, x, z);

	ColliedWithQuadBox(rootNode, 0, mesh);

	//for (int i = 0; i < mesh.size(); i++)
	//	ErrorLog::Log(std::to_string(mesh[i].getBoundingBox()));

	//ErrorLog::Log(std::to_string(inside));
	//ErrorLog::Log(std::to_string(collidedWithBox));
	//for (int i = 0; i < found.size(); i++)
	//	ErrorLog::Log(std::to_string(found[i]));

	return true;
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
					nrOf++;
				}

				if (depth < 3)
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
					nrOf++;
				}

				if (depth < 3)
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
					nrOf++;
				}

				if (depth < 3)
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
					nrOf++;
				}

				if (depth < 3)
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

void QuadTree::ColliedWithQuadBox(Node* node, int depth, std::vector<Mesh>& mesh)
{

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				if (depth < 3)
				{
					ColliedWithQuadBox(node->upLeft, depth + 1, mesh);
				}

				break;
			}
			case 1:
			{
				if (depth < 3)
				{
					ColliedWithQuadBox(node->upRight, depth + 1, mesh);
				}

				break;
			}
			case 2:
			{
				if (depth < 3)
				{
					ColliedWithQuadBox(node->downLeft, depth + 1, mesh);
				}

				break;
			}
			case 3:
			{
				if (depth < 3)
				{
					ColliedWithQuadBox(node->downRight, depth + 1, mesh);
				}

				break;
			}
		}
	}

	if (node->nodePoint != nullptr && depth > 2)
	{
		inside++;
		for (int i = 0; i < mesh.size(); i++)
		{
			if (mesh[i].getBoundingBox().Contains(node->nodePoint->box) != 0)
			{
				node->nodePoint->mesh.push_back(mesh[i]);
				collidedWithBox++;
				found.push_back(mesh[i].getUniqueId());
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
				if (depth < 3)
				{
					ColliedWithViewFrustom(node->upLeft, depth + 1, frosum);
				}

				break;
			}
			case 1:
			{
				if (depth < 3)
				{
					ColliedWithViewFrustom(node->upRight, depth + 1, frosum);
				}
			}
			case 2:
			{
				if (depth < 3)
				{
					ColliedWithViewFrustom(node->downLeft, depth + 1, frosum);
				}

				break;
			}
			case 3:
			{
				if (depth < 3)
				{
					ColliedWithViewFrustom(node->downRight, depth + 1, frosum);
				}
				break;
			}
		}
	}

	if (node->nodePoint != nullptr && depth > 2)
	{
		for (int i = 0; i < node->nodePoint->mesh.size(); i++)
		{
			if (frosum.Contains(node->nodePoint->mesh[i].getBoundingBox()) != 0)
			{
				inViewFrustom.push_back(node->nodePoint->mesh[i]);
			}
		}
	}

}

std::vector<int> QuadTree::AllInViewFrustom(XMMATRIX viewProj)
{
	inViewFrustom.clear();
	DirectX::BoundingFrustum frosum;
	DirectX::XMMATRIX inverseViewMatrix = DirectX::XMMatrixInverse(nullptr, viewProj);
	DirectX::BoundingFrustum::CreateFromMatrix(frosum, viewProj, false);

	ColliedWithViewFrustom(rootNode, 0, frosum);
	//rootNode->box.CreateFromPoints()
	std::vector<int> unique;
	for (int i = 0; i < inViewFrustom.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < unique.size(); j++) 
		{
			if (unique[j] == inViewFrustom[i].getUniqueId())
				found = true;
		}

		if (!found)
			unique.push_back(inViewFrustom[i].getUniqueId());
	}


	return unique;
}


