Texture2D Ambient : register(t0);
Texture2D Deffuse : register(t1);
Texture2D Specular : register(t2);
Texture2D DepthTexture : register(t3);
Texture2D DepthTextureTwo : register(t4);
Texture2D DepthTextureThree : register(t5);
Texture2D DepthTextureFour : register(t6);
SamplerState Sampler : SAMPLER : register(s0);
SamplerState SampleDepth : SAMPLER : register(s1);

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : normal;
	float3 posWS : WS_POSITION;
	float4 posLight : LIGHTPOS;
	float4 posLightTwo : LIGHTPOSTWO;
	float4 posLightThree : LIGHTPOSTHREE;
	float4 posLightFour : LIGHTPOSFOUR;
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
	//// normalize
	//input.posLight = normalize(input.posLight);
	//input.posLightTwo = normalize(input.posLightTwo);
	//input.posLightThree = normalize(input.posLightThree);
	//input.posLightFour = normalize(input.posLightFour);
	
	// ndv
	input.posLight.xy /= input.posLight.w;
	input.posLightTwo.xy /= input.posLightTwo.w;
	input.posLightThree.xy /= input.posLightThree.w;
	input.posLightFour.xy /= input.posLightFour.w;
	
	//input.posLight.xy /= input.posLight.w;
	float2 smTex = float2((0.5f * input.posLight.x) + 0.5f, (-0.5f * input.posLight.y) + 0.5f);
	float2 smTex2 = float2((0.5f * input.posLightTwo.x) + 0.5f, (-0.5f * input.posLightTwo.y) + 0.5f);
	float2 smTex3 = float2((0.5f * input.posLightThree.x) + 0.5f, (-0.5f * input.posLightThree.y) + 0.5f);
	float2 smtex4 = float2((0.5f * input.posLightFour.x) + 0.5f, (-0.5f * input.posLightFour.y) + 0.5f);
	
	float depth = input.posLight.z / input.posLight.w;
	float depth2 = input.posLightTwo.z / input.posLightTwo.w;
	float depth3 = input.posLightThree.z / input.posLightThree.w;
	float depth4 = input.posLightFour.z / input.posLightFour.w;
	float Epsilon = 0.00125f;
	
	float t0 = (DepthTexture.Sample(SampleDepth, smTex + float2(0.0f, 0.0f)).r + Epsilon < depth) ? 0.0f : 1.0f;
	float t1 = (DepthTextureTwo.Sample(SampleDepth, smTex2 + float2(0.0f, 0.0f)).r + Epsilon < depth2) ? 0.0f : 1.0f;
	float t2 = (DepthTextureThree.Sample(SampleDepth, smTex3 + float2(0.0f, 0.0f)).r + Epsilon < depth3) ? 0.0f : 1.0f;
	float t3 = (DepthTextureFour.Sample(SampleDepth, smtex4 + float2(0.0f, 0.0f)).r + Epsilon < depth4) ? 0.0f : 1.0f;
	
	//float2 texelPosition = smTex * 1024.0f;

	//float2 leps = frac(texelPosition);
    
	//float shadow = lerp(lerp(t0, t1, leps.x), lerp(t2, t3, leps.x), leps.y);
	float shadow = (t0 + t1 + t2 + t3) / 4;
	if (shadow <= 0.3f)
		shadow = 0.3f;
	
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