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

struct vsOutPut
{
	float4 posWS : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 ambinetComponent : SV_TARGET2;
	float4 diffuseComponent : SV_TARGET3;
	float4 specularComponent : SV_TARGET4;
	float4 lightPosition : SV_TARGET5;
};

vsOutPut main(PixelShaderInput input) : SV_TARGET
{
	vsOutPut output;
	
	output.posWS = float4(input.posWS, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.ambinetComponent = Ambient.Sample(Sampler, input.uv).rgba;
	output.diffuseComponent = Deffuse.Sample(Sampler, input.uv).rgba;
	output.specularComponent = Specular.Sample(Sampler, input.uv).rgba;
	output.lightPosition = float4(lightPosition, shininess);
	
	return output;
}