#include "QuadTree.h"

void QuadTree::SetupBoundingBoxCenter(Node*& node, float& x, float& z, int index)
{
	if (index == 0) // top left
	{
		x = node->boundingBoxCenter.x - x;
		z = node->boundingBoxCenter.y + z;
	}
	else if (index == 1) // top right
	{
		x = node->boundingBoxCenter.x + x;
		z = node->boundingBoxCenter.y + z;
	}
	else if (index == 2) // bottom left
	{
		x = node->boundingBoxCenter.x - x;
		z = node->boundingBoxCenter.y - z;
	}
	else if (index == 3)
	{
		x = node->boundingBoxCenter.x + x;
		z = node->boundingBoxCenter.y - z;
	}
}

void QuadTree::SetupBoundingBox(Node*& node, float x, float z)
{
	// create size of boundingBox
	XMVECTOR smallest = XMVectorSet(0.0f, -20.f, 0.0f, 0.0f);
	XMVECTOR biggest = XMVectorSet(node->boundingBoxSize.x, 20.0f, node->boundingBoxSize.y, 0.0f);

	// setting up nodePoint
	node->nodePoint = new NodePoint();
	DirectX::BoundingBox::CreateFromPoints(node->nodePoint->box, smallest, biggest);

	DirectX::XMMATRIX WMBB = XMMatrixIdentity();
	XMMATRIX midPoint = XMMatrixTranslation(x, 0.0f, z);

	/*node->nodePoint->box.Transform(node->nodePoint->box, )*/
	node->nodePoint->box.Transform(node->nodePoint->box, midPoint);
}



void QuadTree::SetupQuadTreeEnpty(Node* node, int depth, float x, float z)
{
	node->depth = depth;

	if (depth > 0)
	{
		node->boundingBoxCenter = XMFLOAT2(x, z);
	}

	x = fabsf(x);
	z = fabsf(z);

	node->boundingBoxSize = XMFLOAT2(x, z);

	x /= 2;
	z /= 2;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0: // Top left
			{
				float xLeft = x;
				float zLeft = z;

				SetupBoundingBoxCenter(node, xLeft, zLeft, i);

				if (depth > 0)
				{
					SetupBoundingBox(node, xLeft, zLeft);
					nrOf++;
				}

				if (depth + 1 < 3)
				{
					node->upLeft = new Node();
					SetupQuadTreeEnpty(node->upLeft, depth + 1, xLeft, zLeft);
				}

				break;
			}
			case 1: // Top right
			{
				float xLeft = x;
				float zLeft = z;

				SetupBoundingBoxCenter(node, xLeft, zLeft, i);

				if (depth > 0)
				{
					SetupBoundingBox(node, xLeft, zLeft);
					nrOf++;
				}

				if (depth + 1 < 3)
				{
					node->upRight = new Node();
					SetupQuadTreeEnpty(node->upRight, depth + 1, xLeft, zLeft);
				}

				break;
			}
			case 2: // bottom left
			{
				float xLeft = x;
				float zLeft = z;

				SetupBoundingBoxCenter(node, xLeft, zLeft, i);

				if (depth > 0)
				{
					SetupBoundingBox(node, xLeft, zLeft);
					nrOf++;
				}

				if (depth + 1 < 3)
				{
					node->downLeft = new Node();
					SetupQuadTreeEnpty(node->downLeft, depth + 1, xLeft, zLeft);
				}

				break;
			}
			case 3:
			{
				float xLeft = x;
				float zLeft = z;

				SetupBoundingBoxCenter(node, xLeft, zLeft, i);

				if (depth > 0)
				{
					SetupBoundingBox(node, xLeft, zLeft);
					nrOf++;
				}

				if (depth + 1 < 3)
				{
					node->downRight = new Node();
					SetupQuadTreeEnpty(node->downRight, depth + 1, xLeft, zLeft);
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
				if (depth != 0)
				{
					for (int i = 0; i < mesh.size(); i++)
					{
						if (mesh[i].getBoundingBox().Contains(node->nodePoint->box) != 0)
						{
							if (depth < 2)
							{
								node->upLeft = new Node();
								ColliedWithQuadBox(node->upLeft, depth + 1, mesh);
								break;
							}

							if (depth == 2)
							{
								node->nodePoint->mesh.push_back(mesh[i]);
								inside++;
							}
						}
					}
				}

				break;
			}
			case 1:
			{
				if (depth != 0)
				{
					for (int i = 0; i < mesh.size(); i++)
					{
						if (mesh[i].getBoundingBox().Contains(node->nodePoint->box) != 0)
						{
							if (depth < 2)
							{
								node->upRight = new Node();
								ColliedWithQuadBox(node->upRight, depth + 1, mesh);
								break;
							}

							if (depth == 2)
								node->nodePoint->mesh.push_back(mesh[i]);
						}
					}
				}

				break;
			}
			case 2:
			{
				if (depth != 0)
				{
					for (int i = 0; i < mesh.size(); i++)
					{
						if (mesh[i].getBoundingBox().Contains(node->nodePoint->box) != 0)
						{
							if (depth < 2)
							{
								node->downLeft = new Node();
								ColliedWithQuadBox(node->downLeft, depth + 1, mesh);
								break;
							}

							if (depth == 2)
								node->nodePoint->mesh.push_back(mesh[i]);
						}
					}
				}

				break;
			}
			case 3:
			{
				if (depth != 0)
				{
					for (int i = 0; i < mesh.size(); i++)
					{
						if (mesh[i].getBoundingBox().Contains(node->nodePoint->box) != 0)
						{
							if (depth < 2)
							{
								node->downRight = new Node();
								ColliedWithQuadBox(node->downRight, depth + 1, mesh);
								break;
							}

							if (depth == 2)
								node->nodePoint->mesh.push_back(mesh[i]);
						}
					}
				}

				break;
			}
		}
	}
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

void QuadTree::ColliedWithViewFrustom(Node* node, int depth, DirectX::BoundingFrustum frosum)
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				if (depth > 1)
				{
					if (frosum.Contains(node->nodePoint->box))
					{
						for (int i = 0; i < node->nodePoint->mesh.size(); i++)
						{
							inViewFrustom.push_back(node->nodePoint->mesh[i]);
						}
					}
				}
				else
				{
						ColliedWithViewFrustom(node->upLeft, depth + 1, frosum);
				}

				break;
			}
			case 1:
			{
				if (depth > 1)
				{
					if (frosum.Contains(node->nodePoint->box))
					{
						for (int i = 0; i < node->nodePoint->mesh.size(); i++)
						{
							inViewFrustom.push_back(node->nodePoint->mesh[i]);
						}
					}
				}
				else
				{
					ColliedWithViewFrustom(node->upRight, depth + 1, frosum);
				}
			}
			case 2:
			{
				if (depth > 1)
				{
					if (frosum.Contains(node->nodePoint->box))
					{
						for (int i = 0; i < node->nodePoint->mesh.size(); i++)
						{
							inViewFrustom.push_back(node->nodePoint->mesh[i]);
						}
					}
				}
				else
				{
					ColliedWithViewFrustom(node->downLeft, depth + 1, frosum);
				}

				break;
			}
			case 3:
			{
				if (depth > 1)
				{
					if (frosum.Contains(node->nodePoint->box))
					{
						for (int i = 0; i < node->nodePoint->mesh.size(); i++)
						{
							inViewFrustom.push_back(node->nodePoint->mesh[i]);
						}
					}
				}
				else
				{
					ColliedWithViewFrustom(node->downRight, depth + 1, frosum);
				}
				break;
			}
		}
	}


}

std::vector<int> QuadTree::AllInViewFrustom(XMMATRIX viewProj)
{
	inViewFrustom.clear();
	DirectX::BoundingFrustum frosum;
	//DirectX::XMMATRIX inverseViewMatrix = DirectX::XMMatrixInverse(nullptr, viewProj);
	DirectX::BoundingFrustum::CreateFromMatrix(frosum, viewProj);

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


