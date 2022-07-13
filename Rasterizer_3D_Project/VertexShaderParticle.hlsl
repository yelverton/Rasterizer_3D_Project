
cbuffer Matrices : register(b0)
{
	float3 pos;
	float direction;
}

struct VertexShaderInput
{
	float3 position : POSITION;
};

float4 main(VertexShaderInput input) : SV_POSITION
{
	return float4(1, 1, 1, 1);
}