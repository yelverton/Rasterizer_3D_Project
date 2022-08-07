#include "Frustom.h"

void Frustom::addTree(Node* node, int depth)
{
	
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				node->downLeft = new Node();
				//node->box.CreateFromPoints(box, )
				if (depth + 1 < 3)
					addTree(node->downLeft, depth + 1);
				break;
			}
			case 1:
			{
				node->downRight = new Node();
				if (depth + 1 < 3)
					addTree(node->downRight, depth + 1);
				break;
			}
			case 2:
			{
				node->upLeft = new Node();
				if (depth + 1 < 3)
					addTree(node->upLeft, depth + 1);
				break;
			}
			case 3:
			{
				node->upRight = new Node();
				if (depth + 1 < 3)
					addTree(node->upRight, depth + 1);
				break;
			}
		}
	}

}


bool Frustom::SetupFrustom(XMMATRIX projection, std::vector<BigSmall> bigSmall, std::vector<XMFLOAT4X4> worldFrustom, std::vector<Mesh> mesh)
{
	for (int i = 0; i < bigSmall.size(); i++)
	{
		objects.push_back(DirectX::BoundingBox());
		DirectX::BoundingBox::CreateFromPoints(objects[i], bigSmall[i].smallest, bigSmall[i].biggest);
		XMMATRIX send = XMLoadFloat4x4(&worldFrustom[i]);
		objects[i].Transform(objects[i], send);
		mesh[i].setBB(objects[i]);
	}

	DirectX::BoundingFrustum frosum;
	DirectX::BoundingFrustum::CreateFromMatrix(frosum, projection);
	DirectX::BoundingBox dude;
	
	XMFLOAT3 origin = frosum.Origin;

	float width = 900, height = 900;
	rootNode = new Node();
	addTree(rootNode, 0);
	AddcolliedWithBoundingBox(rootNode, 0, mesh);

	return true;
}

void Frustom::GetcolliedWithFrustom(Node* node, int depth, DirectX::BoundingFrustum frosum)
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			if (depth + 1 < 3)
				GetcolliedWithFrustom(node->downLeft, depth + 1, frosum);
			break;
		}
		case 1:
		{
			if (depth + 1 < 3)
				GetcolliedWithFrustom(node->downRight, depth + 1, frosum);
			break;
		}
		case 2:
		{
			if (depth + 1 < 3)
				GetcolliedWithFrustom(node->upLeft, depth + 1, frosum);
			break;
		}
		case 3:
		{
			if (depth + 1 < 3)
				GetcolliedWithFrustom(node->upRight, depth + 1, frosum);
			break;
		}
		}
	}

	if (depth == 2)
	{
		if (frosum.Contains(node->box))
		{
			for (int i = 0; i < node->mesh.size(); i++)
			{
				currentMesh.push_back(node->mesh[i]);
			}
		}
	}

}


void Frustom::AddcolliedWithBoundingBox(Node* node, int depth, std::vector<Mesh> mesh)
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			for (int i = 0; i < mesh.size(); i++)
			{
				if (mesh[i].getBB().Contains(node->box) != 0)
				{
					node->mesh.push_back(mesh[i]);
					taken.push_back(mesh[i].getBB());
				}
			}
			if (depth + 1 < 3)
				AddcolliedWithBoundingBox(node->downLeft, depth + 1, mesh);
			break;
		}
		case 1:
		{
			for (int i = 0; i < mesh.size(); i++)
			{
				if (mesh[i].getBB().Contains(node->box) != 0)
				{
					node->mesh.push_back(mesh[i]);
					taken.push_back(mesh[i].getBB());
					if (depth + 1 < 3)
						AddcolliedWithBoundingBox(node->downRight, depth + 1, mesh);
				}
			}
			break;
		}
		case 2:
		{
			for (int i = 0; i < mesh.size(); i++)
			{
				if (mesh[i].getBB().Contains(node->box) != 0)
				{
					node->mesh.push_back(mesh[i]);
					taken.push_back(mesh[i].getBB());
					if (depth + 1 < 3)
						AddcolliedWithBoundingBox(node->upLeft, depth + 1, mesh);
				}
			}
			break;
		}
		case 3:
		{
			for (int i = 0; i < mesh.size(); i++)
			{
				if (mesh[i].getBB().Contains(node->box) != 0)
				{
					node->mesh.push_back(mesh[i]);
					taken.push_back(mesh[i].getBB());
					if (depth + 1 < 3)
						AddcolliedWithBoundingBox(node->upRight, depth + 1, mesh);
				}
			}
			break;
		}
		}
		
	}
}

std::vector<int> Frustom::getFrustom(XMMATRIX viewProj)
{
	currentMesh.clear();
	DirectX::BoundingFrustum frosum;
	//DirectX::XMMATRIX inverseViewMatrix = DirectX::XMMatrixInverse(nullptr, viewProj);
	DirectX::BoundingFrustum::CreateFromMatrix(frosum, viewProj);

	GetcolliedWithFrustom(rootNode, 0, frosum);
	//rootNode->box.CreateFromPoints()
	std::vector<int> unique;
	for (int i = 0; i < currentMesh.size(); i++) 
	{
		bool found = false;
		for (int j = 0; j < unique.size(); j++) 
		{
			if (unique[j] == currentMesh[i].getUniqueId())
				found = true;
		}

		if (!found)
			unique.push_back(currentMesh[i].getUniqueId());
	}


	return unique;
}


