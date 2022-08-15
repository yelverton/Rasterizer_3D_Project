Texture2D<float4> posWS : register(t0);
Texture2D<float4> normal : register(t1);
Texture2D<float4> baseColour : register(t2);
Texture2D<float4> ambinetComponent : register(t3);
Texture2D<float4> diffuseComponent : register(t4);
Texture2D<float4> specularComponent : register(t5);

RWTexture2D<unorm float4> backBuffer : register(u0);

cbuffer light : register(b0)
{
	float3 lightPosition;
	float shininess;
};

cbuffer cam : register(b1)
{
	float3 cameraPosition;
	float padding0;
};

cbuffer lightTwo : register(b2)
{
	float3 lightPositionTwo;
	float shininessTwo;
};

cbuffer lightThree : register(b3)
{
	float3 lightPositionThree;
	float shininessThree;
};

cbuffer lightFour : register(b4)
{
	float3 lightPositionFour;
	float shininessFour;
};

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(DTid.x, DTid.y, DTid.z);
	
	float3 Normal = normalize(normal.Load(location).xyz);
	float3 Pos = posWS.Load(location).xyz;
	
	float3 camToPic = normalize(cameraPosition - Pos);
	float3 lightDir = normalize(lightPosition - Pos);
	float3 lightDirTwo = normalize(lightPositionTwo - Pos);
	float3 lightDirThree = normalize(lightPositionThree - Pos);
	float3 lightDirFour = normalize(lightPositionFour - Pos);
	
	// Ambient
	float3 Ambient = ambinetComponent.Load(location).xyz;
	
	float3 diffuseLevel = dot(Normal, -lightDir);
	float3 diffuseLevelTwo = dot(Normal, -lightDirTwo);
	float3 diffuseLevelThree = dot(Normal, -lightDirThree);
	float3 diffuseLevelFour = dot(Normal, -lightDirFour);
	float3 Diffuse = diffuseComponent.Load(location).xyz * diffuseLevel * diffuseLevelTwo * diffuseLevelThree * diffuseLevelFour;
	
	// Specular
	float3 reflectDir = normalize(reflect(lightPosition, Normal));
	float specularConponent = pow(max(dot(-lightDir, reflectDir), 0.0f), shininess);
	
	float3 reflectDirTwo = normalize(reflect(lightPositionTwo, Normal));
	float specularConponentTwo = pow(max(dot(-lightDirTwo, reflectDirTwo), 0.0f), shininess);
	
	float3 reflectDirThree = normalize(reflect(lightPositionThree, Normal));
	float specularConponentThree = pow(max(dot(-lightDirThree, reflectDirThree), 0.0f), shininess);
	
	float3 reflectDirFour = normalize(reflect(lightPositionFour, Normal));
	float specularConponentFour = pow(max(dot(-lightDirFour, reflectDirFour), 0.0f), shininess);
	
	float3 Specular = specularComponent.Load(location).xyz * specularConponent * specularConponentTwo * 
	specularConponentThree * specularConponentFour;

	backBuffer[DTid.xy] = (float4(Ambient, 1.0f) + float4(Diffuse, 1.0f) + float4(Specular, 1.0f)) * diffuseComponent.Load(location).w;

}