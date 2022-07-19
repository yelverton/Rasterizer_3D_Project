Texture2D<float4> posWS : register(t0);
Texture2D<float4> normal : register(t1);
Texture2D<float4> baseColour : register(t2);
Texture2D<float4> ambinetComponent : register(t3);
Texture2D<float4> diffuseComponent : register(t4);
Texture2D<float4> specularComponent : register(t5);

RWTexture2D<unorm float4> backBuffer : register(u0);

//cbuffer light : register(b0)
//{
//	float3 lightPosition;
//	float shininess;
//};

//cbuffer cam : register(b1)
//{
//	float3 cameraPosition;
//	float padding0;
//};

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(DTid.x, DTid.y, DTid.z);
	
	//float3 Normal = normalize(normal.Load(location).xyz);
	//float3 Pos = posWS.Load(location).xyz;
	
	//float3 camToPic = normalize(Pos - cameraPosition);
	//float3 lightDir = normalize(Pos - lightPosition);
	
	//// Ambient
	//float3 Ambient = ambinetComponent.Load(location).xyz;
	
	//// Diffuse
	//float3 diffuseLevel = dot(Normal, -lightDir);
	float3 Diffuse = diffuseComponent.Load(location).xyz/* * diffuseLevel*/;

	//// kan vara fel
	//float3 vecBetweenWPosLight = normalize(lightPosition - Pos);
	//float diffuselight = clamp(dot(Normal, vecBetweenWPosLight), 0.0f, 1.0f); // * lightColor;
	//float3 diffuselightClr = Ambient * diffuselight;
	
	//// Specular
	//float3 reflectDir = normalize(reflect(lightPosition, Normal));
	//float specularConponent = pow(max(dot(-lightDir, reflectDir), 0.0f), shininess);
	//float3 Specular = specularComponent.Load(location).xyz * specularConponent * 10.0f;

	//float3 color = Ambient * diffuselightClr;
	
	backBuffer[DTid.xy] = float4(Diffuse, 1.0f) * diffuseComponent.Load(location).w;

}