Texture2D Ambient : register(t0);
Texture2D Deffuse : register(t2);
Texture2D Specular : register(t1);
SamplerState Sampler : SAMPLER : register(s0);

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 posWS : WS_POSITION;
};

cbuffer Light : register(b0)
{
	float3 lightPosition;
	float shininess;
};

cbuffer cam : register(b1)
{
	float3 cameraPosition;
	float padding;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 ambient = Ambient.Sample(Sampler, input.uv);
	return float4(ambient, 1.0f);
}