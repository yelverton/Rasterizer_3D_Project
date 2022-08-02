Texture2D Ambient : register(t0);
Texture2D Diffuse : register(t1);
Texture2D Specular : register(t2);
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
	float3 ambient = Ambient.Sample(Sampler, input.uv);
	float3 diffuse = Diffuse.Sample(Sampler, input.uv);
	float3 specular = Specular.Sample(Sampler, input.uv);

	//return float4(ambient + diffuse + specular, 1.0f);
	return float4(1.0f, 1.0f, 1.0f, 1.0f);

}