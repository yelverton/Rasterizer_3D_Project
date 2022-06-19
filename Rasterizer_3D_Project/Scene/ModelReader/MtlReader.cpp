#include "MtlReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

bool CreateShaderResourView(std::string mtlPictures, ID3D11ShaderResourceView*& shader, ID3D11Device* device)
{
	int width, height, channel;
	unsigned char* picture = stbi_load(mtlPictures.c_str(), &width, &height, &channel, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = (UINT)width;
	textureDesc.Height = (UINT)height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource;
	ZeroMemory(&subResource, sizeof(D3D11_SUBRESOURCE_DATA));

	subResource.pSysMem = picture;
	subResource.SysMemPitch = (UINT)width * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = device->CreateTexture2D(&textureDesc, &subResource, &texture);
	if (FAILED(hr)) {
		ErrorLog::Log(hr, "Failed to create texture2D");
		return false;
	}

	hr = device->CreateShaderResourceView(texture.Get(), nullptr, &shader);
	if (FAILED(hr)) {
		ErrorLog::Log(hr, "Failed to create shaderResourceView");
		return false;
	}

	stbi_image_free(picture);
	return true;
}

bool mtlReader(std::string mtlFile, ID3D11ShaderResourceView*& ambient, ID3D11ShaderResourceView*& diffuse,
	ID3D11ShaderResourceView*& specular, std::string readArea, std::vector<float>& specularExponent, ID3D11Device* device)
{
	std::vector<std::string> ambientStrVector;
	std::vector<std::string> diffuseStrVector;
	std::vector<std::string> specularStrVector;

	string ambientStr = "missingTexture";
	string diffuseStr = "missingTexture";
	string specularStr = "missingTexture";


	std::ifstream file("Scene/models/" + mtlFile);
	if (!file.is_open())
	{
		ErrorLog::Log("Failed to open " + mtlFile);
		return false;
	}

	string line, word;
	bool okayRead = false;

	//istringstream iss;
	//while (!file.eof())
	//{
	//	getline(file, line);
	//	if (file.is_open())
	//	{
	//		iss.clear();
	//		iss.str(line);
	//		if (iss.good())
	//		{
	//			iss >> word;
	//			if (word == "newmtl")
	//			{
	//				iss >> word; 
	//				if (word == readArea)
	//					okayRead = true;
	//				else
	//					okayRead = false;
	//			}
	//			else if (okayRead)
	//			{
	//				if (word == "map_Kd") {
	//					iss >> word;
	//					ambientStr = word;
	//				}
	//				else if (word == "map_Ks") {
	//					iss >> word;
	//					diffuseStr = word;
	//				}
	//				else if (word == "map_Ka") {
	//					iss >> word;
	//					specularStr = word;
	//				}
	//				else if (word == "Ns")
	//				{
	//					iss >> word;
	//					specularExponent.push_back(stof(word));
	//				}
	//			}
	//		}

	//	}
	//}

	if (ambientStr == "missingTexture" || diffuseStr == "missingTexture" || specularStr == "missingTexture")
	{
		ambientStr = "missingTexture";
		diffuseStr = "missingTexture";
		specularStr = "missingTexture";
	}

	CreateShaderResourView("Scene/Models/Texture/" + ambientStr + ".png", ambient, device);
	CreateShaderResourView("Scene/Models/Texture/" + ambientStr + ".png", diffuse, device);
	CreateShaderResourView("Scene/Models/Texture/" + ambientStr + ".png", specular, device);

    return true;
}

