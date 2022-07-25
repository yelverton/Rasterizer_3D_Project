cbuffer Matrices : register(b0)
{
	matrix world;
}

cbuffer cam : register(b1)
{
	matrix view;
	matrix projection;
}

cbuffer camPos : register(b2)
{
	float3 cameraPosition;
	float padding;
}

struct VertexShaderInput
{
	float3 position : POSITION;
};

float4 main(VertexShaderInput input) : SV_POSITION
{
	float4x4 viewProjection = mul(view, projection);
	//float4x4 worldWiewProjection = mul(world, mul(view, projection)); Kanske är den man ska ha vet ej!
	return mul(viewProjection, float4(input.position + cameraPosition, 1.0f)).xyww;
	
}