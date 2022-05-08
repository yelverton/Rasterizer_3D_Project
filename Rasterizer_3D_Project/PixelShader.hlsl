//Texture2D Ambient : TEXTURE : register(t0);
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
	input.normal = normalize(input.normal);
	float3 camToPicture = normalize(input.posWS - cameraPosition);
	float3 lightDirecion = normalize(input.posWS - lightPosition);

	//float3 ambient = Ambient.Sample(Sampler, input.uv);
	float3 ambient = { 1.0f, 0.0f, 0.0f };
    // diffuse
	float3 diffuseLevel = (dot(input.normal, -lightDirecion));
	float3 diffuse = diffuseLevel;
    
    //specular
	float3 reflectDirection = normalize(reflect(lightPosition, input.normal));
	float specularConponent = pow(max(dot(camToPicture, reflectDirection), 0.0f), shininess);
	float3 specular = specularConponent;
    
	float3 color = ambient + diffuse + specular;
	return float4(ambient, 1.0f);
}