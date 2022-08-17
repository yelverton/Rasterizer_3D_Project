Texture2D<float4> posWS : register(t0);
Texture2D<float4> normal : register(t1);
Texture2D<float4> baseColour : register(t2);
Texture2D<float4> ambinetComponent : register(t3);
Texture2D<float4> diffuseComponent : register(t4);
Texture2D<float4> specularComponent : register(t5);

RWTexture2D<unorm float4> backBuffer : register(u0);

cbuffer light : register(b0)
{
	float3 lightDirection;
	float padding;
};

cbuffer cam : register(b1)
{
	float3 cameraPosition;
	float padding0;
};

cbuffer lightTwo : register(b2)
{
	float3 posTwo;
	float ranageTwo;
	float3 dirTwo;
	float coneTwo;
	float3 cSpotTwo;
	float paddingTwo;
	float3 attTwo;
	float paddingTwoTwo;
};

cbuffer lightThree : register(b3)
{
	float3 posThree;
	float ranageThree;
	float3 dirThree;
	float coneThree;
	float3 cSpotThree;
	float paddingThree;
	float3 attThree;
	float paddingThreeThree;
};

cbuffer lightFour : register(b4)
{
	float3 posFour;
	float ranageFour;
	float3 dirFour;
	float coneFour;
	float3 cSpotFour;
	float paddingFour;
	float3 attFour;
	float paddingFourFour;
};

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	// setup
	int3 location = int3(DTid.x, DTid.y, DTid.z);
	float3 Normal = normalize(normal.Load(location).xyz);
	float3 Pos = posWS.Load(location).xyz;
	float3 camToPic = normalize(Pos - cameraPosition);
	
	// textures
	float3 Ambient = ambinetComponent.Load(location).xyz * 0.4f;
	float3 Diffuse = diffuseComponent.Load(location).xyz;
	float3 Specular = specularComponent.Load(location).xyz;
	
	////// diffuse (direction light)
	float3 lightDir = -normalize(lightDirection);
	float3 diffuseLevel = max(dot(Normal, lightDir), 0.0f);
	
	// specular (direction light)
	float3 reflectDirFour = normalize(reflect(lightDirection, Normal));
	float3 specCompOne = pow(max(dot(reflectDirFour, -camToPic), 0.0f), 50);
	
	//spotlight(LightTwo)
	
	float3 lightToPixelTwo = posTwo - Pos;
	lightToPixelTwo = normalize(lightToPixelTwo);
	float lightStrenghtTwo = length(lightToPixelTwo);
	float diffuseLevelTwo = 0.0f;
	float specCompTwo = 0.0f;
	
	if (lightStrenghtTwo <= ranageTwo)
	{
		lightToPixelTwo /= lightStrenghtTwo;
		float amountLightTwo = dot(lightToPixelTwo, Normal);
		
		if (amountLightTwo > 0.0f)
		{
			Diffuse /= (attTwo[0] + (attTwo[1] * lightStrenghtTwo) + (attTwo[2] * (lightStrenghtTwo * lightStrenghtTwo)));
			diffuseLevelTwo = pow(max(dot(-lightToPixelTwo, dirTwo), 0.0f), coneTwo);
			
			float3 recTwo = normalize(reflect(-dirTwo, Normal));
			specCompTwo = pow(max(dot(recTwo, lightToPixelTwo), 0.0f), 120.0f);
		}
	}
	
	// spotlight (Light Three)
	float3 lightToPixelThree = posThree - Pos;
	lightToPixelThree = normalize(lightToPixelThree);
	float lightStrenghtThree = length(lightToPixelThree);
	float diffuseLevelThree = 0.0f;
	float specCompThree = 0.0f;
	
	if (lightStrenghtThree <= ranageThree)
	{
		lightToPixelThree /= lightStrenghtThree;
		float amountLightThree = dot(lightToPixelThree, Normal);
		
		if (amountLightThree > 0.0f)
		{
			Diffuse /= (attThree[0] + (attThree[1] * lightStrenghtThree) + (attThree[2] * (lightStrenghtThree * lightStrenghtThree)));
			diffuseLevelThree = pow(max(dot(-lightToPixelThree, dirThree), 0.0f), coneThree);
			
			float3 recThree = normalize(reflect(-dirThree, Normal));
			specCompTwo = pow(max(dot(recThree, lightToPixelThree), 0.0f), 120.0f);
		}
	}
	
	// spotlight (Light Four)
	float3 lightToPixelFour = posFour - Pos;
	lightToPixelFour = normalize(lightToPixelFour);
	float lightStrenghtFour = length(lightToPixelFour);
	float diffuseLevelFour = 0.0f;
	float specCompFour = 0.0f;
	
	if (lightStrenghtFour <= ranageFour)
	{
		lightToPixelFour /= lightStrenghtFour;
		float amountLightFour = dot(lightToPixelFour, Normal);
		
		if (amountLightFour > 0.0f)
		{
			Diffuse /= (attFour[0] + (attFour[1] * lightStrenghtFour) + (attFour[2] * (lightStrenghtFour * lightStrenghtFour)));
			diffuseLevelFour = pow(max(dot(-lightToPixelFour, dirFour), 0.0f), coneFour);
			
			float3 recFour = normalize(reflect(-dirFour, Normal));
			specCompTwo = pow(max(dot(recFour, lightToPixelFour), 0.0f), 120.0f);
		}
	}
	
	// diffuse + specular (direction light) sumbit
	//Diffuse = Diffuse * diffuseLevel;
	
	//// Diffuse 
	//float3 DiffuseInfo0 = Diffuse * diffuseLevel;
	//float3 DiffuseInfo1 = Diffuse * diffuseLevelTwo;
	//float3 DiffuseInfo2 = Diffuse * diffuseLevelThree;
	//float3 DiffuseInfo3 = Diffuse * diffuseLevelFour;
	//float3 finalDiffuse = (DiffuseInfo0 + DiffuseInfo1 + DiffuseInfo2 + DiffuseInfo3) * ambinetComponent.Load(location).w;
	//// Specular 
	//float3 SpecularInfo0 = Specular * specCompOne * posWS.Load(location).w;
	//float3 SpecularInfo1 = Specular * specCompTwo * ambinetComponent.Load(location).w;
	//float3 SpecularInfo2 = Specular * specCompThree * diffuseComponent.Load(location).w;
	//float3 SpecularInfo3 = Specular * specCompFour * specularComponent.Load(location).w;
	//float3 finalSpecular = SpecularInfo0 + SpecularInfo1 + SpecularInfo2 + SpecularInfo3;
	
	//// output color
	//float3 output = Ambient + finalDiffuse + finalSpecular;
	
	//Diffuse *= diffuseLevel + diffuseLevelTwo + diffuseLevelThree + diffuseLevelFour;
	//Specular *= specCompOne + specCompTwo + specCompThree + specCompFour;

	float3 DiffuseUse = (Diffuse * diffuseLevel + Diffuse * diffuseLevelTwo + Diffuse * diffuseLevelThree + Diffuse * diffuseLevelFour);
	float specularUse = Specular * specCompOne + Specular * specCompTwo + Specular * specCompThree + Specular * specCompFour;
	float3 finalUse = (Ambient + DiffuseUse + specularUse) * ambinetComponent.Load(location).w;
	
	//float3 finalUse2 = (Diffuse * ambinetComponent.Load(location).w) + Specular + Ambient;
	//float3 finalUse3 = finalUse + finalUse2;
	////Specular = Specular * specCompTwo;
	
	//backBuffer[DTid.xy] = float4(finalUse3, 1.0f) + (float4(Diffuse, 1.0f) * posWS.Load(location).w) + float4(Specular, 1.0f);
	backBuffer[DTid.xy] = float4(finalUse, 1.0f);
}