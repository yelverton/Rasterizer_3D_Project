cbuffer Matrices : register(b0)
{
	matrix world;
}

cbuffer Matrices : register(b1)
{
	matrix viewProj;
}

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
};

float4 main(VertexShaderInput input) : SV_POSITION
{
	float4 output;
	float4x4 worldWiewProjection = mul(world, viewProj);
	output = mul(float4(input.position, 1.0f), worldWiewProjection);
	return output;
}