#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <iostream>

bool setupTesselering(ID3D11HullShader*& hShader, ID3D11DomainShader*& dShader, ID3D11RasterizerState*& resterizerState);