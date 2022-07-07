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
	float4 posLight : LIGHTPOS;
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
	input.posLight.xy /= input.posLight.w;
	float2 smTex = float2(0.5f * input.posLight.x + 0.5f, -0.5f * input.posLight.y + 0.5f);
	float depth = input.posLight.z / input.posLight.w;
	float Epsilon = 0.000125f;
	
	float dx = 1.0f / 2048.0f;
	float t0 = (DepthTexture.Sample(SampleDepth, smTex + float2(0.0f, 0.0f)).r + Epsilon < depth) ? 0.0f : 1.0f;
	float t1 = (DepthTexture.Sample(SampleDepth, smTex + float2(dx, 0.0f)).r + Epsilon < depth) ? 0.0f : 1.0f;
	float t2 = (DepthTexture.Sample(SampleDepth, smTex + float2(0.0f, dx)).r + Epsilon < depth) ? 0.0f : 1.0f;
	float t3 = (DepthTexture.Sample(SampleDepth, smTex + float2(dx, dx)).r + Epsilon < depth) ? 0.0f : 1.0f;
	
	float2 texelPosition = smTex * 2048.0f;

	float2 leps = frac(texelPosition);
    
	float shadow = lerp(lerp(t0, t1, leps.x), lerp(t2, t3, leps.x), leps.y);
	//float shadow = (t0 + t1 + t2 + t3) / 4;
	if (shadow <= 0.3f)
		shadow = 0.3f;
	
	//if (shadow < 0.0001)
	//	shadow = 0.0f;
	
	vsOutPut output;
	
	output.posWS = float4(input.posWS, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.baseColour = float4(1.0f, 0.0f, 0.0f, 0.0f); // Behöver lägga till specular exponent fråga om det är shiness kan vara ej :D
	output.ambinetComponent = Ambient.Sample(Sampler, input.uv).rgba;
	output.ambinetComponent.w = shadow;
	output.diffuseComponent = Deffuse.Sample(Sampler, input.uv).rgba;
	output.specularComponent = Specular.Sample(Sampler, input.uv).rgba;
	
	return output;
}