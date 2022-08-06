SamplerState Sampler : SAMPLER : register(s0);
TextureCube texCube : TEXCUBE : register(t0);

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
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
	float3 location = normalize(reflect(vecObject, normal));
	
	return float4(texCube.Sample(Sampler, location).rgb, 1.0f);
}