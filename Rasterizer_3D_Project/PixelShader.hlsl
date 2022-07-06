Texture2D Ambient : register(t0);
Texture2D Deffuse : register(t1);
Texture2D Specular : register(t2);
Texture2D DepthTexture : register(t3);
SamplerState Sampler : SAMPLER : register(s0);
SamplerState SampleDepth : SAMPLER : register(s1);

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 posWS : WS_POSITION;
};

struct vsOutPut
{
	float4 posWS : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 baseColour : SV_TARGET2;
	float4 ambinetComponent : SV_TARGET3;
	float4 diffuseComponent : SV_TARGET4;
	float4 specularComponent : SV_TARGET5;
};

vsOutPut main(PixelShaderInput input) : SV_TARGET
{
	vsOutPut output;
	
	output.posWS = float4(input.posWS, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.baseColour = float4(1.0f, 0.0f, 0.0f, 0.0f); // Behöver lägga till specular exponent fråga om det är shiness kan vara ej :D
	output.ambinetComponent = Ambient.Sample(Sampler, input.uv).rgba;
	output.diffuseComponent = Deffuse.Sample(Sampler, input.uv).rgba;
	output.specularComponent = Specular.Sample(Sampler, input.uv).rgba;
	
	return output;
}