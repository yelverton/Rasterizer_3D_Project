cbuffer Matrices : register(b0)
{
	matrix world;
}

cbuffer camera : register(b1)
{
	matrix view;
	matrix projection;
}

cbuffer LightVP : register(b2)
{
	matrix lightView;
	matrix lightProjection;
}

cbuffer LightVPTwo : register(b3)
{
	matrix lightViewTwo;
	matrix lightProjectionTwo;
}

cbuffer LightVPThree : register(b4)
{
	matrix lightViewThree;
	matrix lightProjectionThree;
}

cbuffer LightVPFour : register(b5)
{
	matrix lightViewFour;
	matrix lightProjectionFour;
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
	float4 posLight : LIGHTPOS;
	float4 posLightTwo : LIGHTPOSTWO;
	float4 posLightThree : LIGHTPOSTHREE;
	float4 posLightFour : LIGHTPOSFOUR;
	float4 shiness : SHINESS;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
	float4x4 worldWiewProjection = mul(world, mul(view, projection));
	
	// Light
	float4x4 lightWorldViewProjection = mul(world, mul(lightView, lightProjection));
	float4x4 lightWorldViewProjectionTwo = mul(world, mul(lightViewTwo, lightProjectionTwo));
	float4x4 lightWorldViewProjectionThree = mul(world, mul(lightViewThree, lightProjectionThree));
	float4x4 lightWorldViewProjectionFour = mul(world, mul(lightViewFour, lightProjectionFour));
	
	output.uv = input.uv;
	output.position = mul(float4(input.position, 1.0f), worldWiewProjection);
	output.normal = normalize(mul(input.normal, (float3x3) world));
	output.posWS = mul(float4(input.position, 1.0f), world).xyz;
	
	// light
	output.posLight = mul(float4(input.position, 1.0f), lightWorldViewProjection);
	output.posLightTwo = mul(float4(input.position, 1.0f), lightWorldViewProjectionTwo);
	output.posLightThree = mul(float4(input.position, 1.0f), lightWorldViewProjectionThree);
	output.posLightFour = mul(float4(input.position, 1.0f), lightWorldViewProjectionFour);
	
	return output;
}