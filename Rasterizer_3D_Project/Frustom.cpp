#include "Frustom.h"

void Frustom::goThrowFrustom(float xAnswer, float zAnswer)
{
	XMVECTOR xPos = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR zPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Contains(frosum);
	}

	for (int i = 0; i < 4; i++) // 15
	{
		frustomNode tempN; 
		tempN.upLeftChild = i;
		tempN.upRightChild = i + 1;
		tempN.downLeftChild = i + 2;
		tempN.upRightChild = i + 3;
		fNode.push_back(tempN);
	}

	for (int i = 16; i < 20; i++) // 15
	{
		frustomNode tempN;
		tempN.nr = i;
		tempN.upRightChild = i + 4 + 4;
		tempN.upRightChild = i + 4 + 4;
		tempN.downLeftChild = i + 4 + 4;
		tempN.upRightChild = i + 4 + 4;
		fNode.push_back(tempN);
	}
}

bool Frustom::SetupFrustom(XMMATRIX projection, std::vector<BigSmall> bigSmall)
{
	DirectX::BoundingFrustum::CreateFromMatrix(frosum, projection);

	for (int i = 0; i < bigSmall.size(); i++)
	{
		DirectX::BoundingBox::CreateFromPoints(objects[i], bigSmall[i].smallest, bigSmall[i].biggest);
	}



	return true;
}


void Frustom::goThrowFrustom()
{
	XMVECTOR getStuff;

	for (int i = 0; i < objects.size(); i++)
	{
		if (frosum.Intersects(objects[i]))
		{
			goThrowFrustom()
		}
	}
}


