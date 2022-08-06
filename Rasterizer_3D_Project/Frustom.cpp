#include "Frustom.h"

void Frustom::addFrustom(Node* node, int depth, int topLeft, int topRight)
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
			{
				Node* bottomLeft = new Node();
				nodeVec.push_back(bottomLeft);
				node->downLeft = bottomLeft;
				if (depth + 1 < 3)
					addFrustom(bottomLeft, depth + 1, 0, 0);
				break;
			}
			case 1:
			{
				Node* bottomRight = new Node();
				nodeVec.push_back(bottomRight);
				node->downRight = bottomRight;
				if (depth + 1 < 3)
					addFrustom(bottomRight, depth + 1, 0, 0);
				break;
			}
			case 2:
			{
				Node* topLeft = new Node();
				nodeVec.push_back(topLeft);
				node->downLeft = topLeft;
				if (depth + 1 < 3)
					addFrustom(topLeft, depth + 1, 0, 0);
				break;
			}
			case 3:
			{
				Node* topRight = new Node();
				nodeVec.push_back(topRight);
				node->upRight = topRight;
				if (depth + 1 < 3)
					addFrustom(topRight, depth + 1, 0, 0);
				break;
			}
		}
	}

}

void CreateTree(Node* node, int depth)
{

}

void Frustom::goThrowFrustom(float xAnswer, float zAnswer)
{
	XMVECTOR xPos = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR zPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Contains(frosum);
	}

	

}

bool Frustom::SetupFrustom(XMMATRIX projection, std::vector<BigSmall> bigSmall, std::vector<XMFLOAT4X4> worldFrustom)
{
	DirectX::BoundingFrustum::CreateFromMatrix(frosum, projection);

	for (int i = 0; i < bigSmall.size(); i++)
	{
		objects.push_back(DirectX::BoundingBox());
		DirectX::BoundingBox::CreateFromPoints(objects[i], bigSmall[i].smallest, bigSmall[i].biggest);
		XMMATRIX send = XMLoadFloat4x4(&worldFrustom[i]);
		objects[i].Transform(objects[i], send);
	}

	Node* nodeTemp = new Node();
	addFrustom(nodeTemp, 0, 0, 0);

	return true;
}



void Frustom::goThrowFrustom()
{
	XMVECTOR getStuff;

	//for (int i = 0; i < objects.size(); i++)
	//{
	//	if (frosum.Intersects(objects[i]))
	//	{
	//		goThrowFrustom()
	//	}
	//}
}

void Frustom::colliedWithBoundingBox(Node* node, int depth, std::vector<BoundingBox> object)
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			if (depth == 2)
			{
				for (int i = 0; i < object.size(); i++)
				{ 
					if (object[i].Contains(node->box))
				}
			}
		}
		case 1:
		{
			Node* bottomRight = new Node();
			nodeVec.push_back(bottomRight);
			node->downRight = bottomRight;
			if (depth + 1 < 3)
				addFrustom(bottomRight, depth + 1, 0, 0);
			break;
		}
		case 2:
		{
			Node* topLeft = new Node();
			nodeVec.push_back(topLeft);
			node->downLeft = topLeft;
			if (depth + 1 < 3)
				addFrustom(topLeft, depth + 1, 0, 0);
			break;
		}
		case 3:
		{
			Node* topRight = new Node();
			nodeVec.push_back(topRight);
			node->upRight = topRight;
			if (depth + 1 < 3)
				addFrustom(topRight, depth + 1, 0, 0);
			break;
		}
		}
	}
}


