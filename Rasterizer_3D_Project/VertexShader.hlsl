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
	float4x4 viewProjectionLight;
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
	output.uv = input.uv;
	output.position = mul(float4(input.position, 1.0f), mul(world, viewProj));
	output.normal = mul(float4(input.normal, 0.0f), world).xyz;
	output.posWS = mul(float4(input.position, 1.0f), world).xyz;
	output.posLight = normalize(mul(mul(float4(input.position, 1.0f), world), mul(world, viewProjectionLight)));
	return output;
}