cbuffer Matrices : register(b0)
{
	matrix world;
}

cbuffer Matrices : register(b1)
{
	matrix view;
	matrix projection;
}

cbuffer LightVP : register(b2)
{
	matrix lightView;
	matrix lightProjection;
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
	
	float4x4 worldWiewProjection = mul(world, mul(view, projection));
	output.uv = input.uv;
	output.position = mul(float4(input.position, 1.0f), worldWiewProjection);
	output.normal = normalize(mul(input.normal, (float3x3) world));
	output.posWS = mul(float4(input.position, 1.0f), world).xyz;
	output.posLight = normalize(mul(mul(float4(input.position, 1.0f), world), mul(lightView, lightProjection)));
	return output;
}