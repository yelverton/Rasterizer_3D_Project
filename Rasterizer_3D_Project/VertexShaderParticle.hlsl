
cbuffer Matrices : register(b0)
{
	matrix world;
}

struct VertexShaderInput
{
	float3 position : POSITION;
};

float4 main(VertexShaderInput input) : SV_POSITION
{
	return mul(float4(input.position, 1.0f), world);
}