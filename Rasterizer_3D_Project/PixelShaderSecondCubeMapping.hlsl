SamplerState Sampler : SAMPLER : register(s0);
TextureCube texCube : TEXCUBE : register(t0);

struct PixelShaderInput
{
	float4 position : POSITION;
	float2 uv : UV;
	float3 normal : normal;
	float3 posWS : WS_POSITION;
};

cbuffer cam : register(b0)
{
	float3 cameraPosition;
	float padding;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 vecObject = normalize(input.posWS - cameraPosition);
	float3 normal = normalize(input.normal);
	float3 location = normalize(reflect(normal, vecObject));
	
	//return float4(texCube.Sample(Sampler, location).rgb, 1.0f);
	return float4(1.0f, 1.0f, 1.0f, 1.0f);

}