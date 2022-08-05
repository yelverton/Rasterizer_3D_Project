Texture2D Ambient : register(t0);
SamplerState Sampler : SAMPLER : register(s0);

struct PixelShaderInput
{
	float4 position : POSITION;
	float2 uv : UV;
	float3 normal : normal;
	float3 posWS : WS_POSITION;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(Ambient.Sample(Sampler, input.uv).rgba);
}