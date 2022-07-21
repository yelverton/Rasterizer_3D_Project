cbuffer Matrices : register(b0)
{
	matrix world;
}

cbuffer light : register(b1)
{
	matrix view;
	matrix projection;
}

struct VertexShaderInput
{
	float3 position : POSITION;
};

float4 main(VertexShaderInput input) : SV_POSITION
{
	matrix worldWiewProjection = mul(world, mul(view, projection));
	return mul(float4(input.position, 1.0f), worldWiewProjection);
	
}