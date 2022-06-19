#include "ObjHelper.h"

XMFLOAT3 extractFloat3(istringstream& iss)
{
	XMFLOAT3 input;
	iss >> input.x;
	iss >> input.y;
	iss >> input.z;

	return input;
}

XMFLOAT2 extractFloat2(istringstream& iss)
{
	XMFLOAT2 input;
	iss >> input.x;
	iss >> input.y;

	return input;
}

vector<XMINT3> extractLetter(istringstream& iss)
{
	vector<XMINT3> input;
	string word;
	int count = 0;
	while (iss.good())
	{
		string index[3];
		iss >> word;
		for (char c : word)
		{
			if (c != '/')
				index[count] += c;
			else
				count++;
		}
		count = 0;
		input.push_back(XMINT3((atoi(index[0].c_str()) - 1), (atoi(index[1].c_str()) - 1), (atoi(index[2].c_str()) - 1)));
	}

	if (input.size() > 3)
	{
		input.erase(input.end() - 1, input.end());
	}

	return input;
}

int checkIfVertexExist(vector<XMINT3>& input, XMINT3 check)
{
	int found = -1;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i].x == check.x && input[i].y == check.y && input[i].z == check.z)
			found = i;
	}

	if (found == -1)
		input.push_back(check);

	return found;
}

bool objReader(std::string modelName, vector<Mesh>& mesh, ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> ambientVec;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> diffuseVec;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> specularVec;

	vector<float> specularExponent;
	std::vector<UINT> indexCount;
	std::vector<UINT> startLocation;

	std::ifstream file("Scene/models/" + modelName + ".obj");
	if (!file.is_open())
	{
		ErrorLog::Log("Failed to open " + modelName + ".obj");
		return false;
	}

	std::vector<SimpleVertex> vertex;
	std::vector<DWORD> indices;

	vector<XMFLOAT3> v; // local space
	vector<XMFLOAT3> vn; // loc
	vector<XMFLOAT2> vt; // uv

	vector<XMINT3> tempINT3; 
	vector<XMINT3> f_ID;

	std::string mtlFile, readArea, line, word;
	int indicesCounter = 0;
	int getNr;

	istringstream iss;
	while (!file.eof())
	{
		getline(file, line);
		if (file.is_open())
		{
			iss.clear();
			iss.str(line);
			if (iss.good())
			{
				iss >> word;
				if (word == "mtllib") iss >> mtlFile;
				if (word == "v") v.push_back(extractFloat3(iss));
				if (word == "vn") vn.push_back(extractFloat3(iss));
				if (word == "vt") vt.push_back(extractFloat2(iss));
				if (word == "usemtl")
				{
					startLocation.push_back(indices.size());
					if (indices.size() > 0)
					{
						indexCount.push_back(indices.size());
						for (int i = 0; i < indexCount.size() - 1; i++)
						{
							indexCount[indexCount.size() - 1] -= indexCount[i];
						}
					}

					ID3D11ShaderResourceView* ambient;
					ID3D11ShaderResourceView* diffuse;
					ID3D11ShaderResourceView* specular;
					
					iss >> readArea;
					mtlReader(mtlFile, ambient, diffuse, specular, readArea, specularExponent, device);

					ambientVec.push_back(ambient);
					diffuseVec.push_back(diffuse);
					specularVec.push_back(specular);

					ambient->Release();
					diffuse->Release();
					specular->Release();
				}
				if (word == "f")
				{
					tempINT3 = extractLetter(iss);

					for (int i = 0; i < tempINT3.size(); i++)
					{
						getNr = checkIfVertexExist(f_ID, tempINT3[i]);
						if (getNr != -1)
						{
							indices.push_back(getNr);
						}
						else 
						{
							indices.push_back(indicesCounter);
							SimpleVertex tempV(v[tempINT3[i].x], vt[tempINT3[i].y], vn[tempINT3[i].z]);
							vertex.push_back(tempV);
							indicesCounter++;
						}
					}

					tempINT3.clear();
				}
			}
		}
	}

	indexCount.push_back(indices.size());
	for (int i = 0; i < indexCount.size() - 1; i++)
	{
		indexCount[indexCount.size() - 1] -= indexCount[i];
	}

	mesh.push_back(Mesh(device, immediateContext, vertex, indices, startLocation, indexCount,
		ambientVec, diffuseVec, specularVec));
	return true;
}
