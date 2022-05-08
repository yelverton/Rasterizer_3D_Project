cbuffer Matrices : register(b0)
{
	matrix world, view, projection;
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
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;

	float4x4 worldWiewProjection = mul(mul(world, view), projection);
	output.uv = input.uv;
	output.position = mul(float4(input.position, 1.0f), worldWiewProjection);
	output.normal = mul(float4(input.normal, 0.0f), world).xyz;
	output.posWS = mul(float4(input.position, 1.0f), world).xyz;
	return output;
}