cbuffer Matrices : register(b0)
{
	matrix world;
}

cbuffer Matrices : register(b1)
{
	matrix viewProj;
}

cbuffer LightVP : register(b2)
{
	matrix viewProjectionLight;
}

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : normal;
	float3 posWS : WS_POSITION;
	float4 posLight : LIGHTPOS;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
	float4x4 worldWiewProjection = mul(world, viewProj);
	output.uv = input.uv;
	output.position = mul(float4(input.position, 1.0f), worldWiewProjection);
	output.normal = mul(float4(input.normal, 0.0f), world).xyz;
	output.posWS = mul(float4(input.position, 1.0f), world).xyz;
	output.posLight = mul(mul(float4(input.position, 1.0f), world), viewProjectionLight);
	return output;
}