Texture2D<float4> posWS : register(t0);
Texture2D<float4> normal : register(t1);
Texture2D<float4> ambinetComponent : register(t2);
Texture2D<float4> diffuseComponent : register(t3);
Texture2D<float4> specularComponent : register(t4);
Texture2D<float4> lightPosition : register(t5);

RWTexture2D<unorm float4> backBuffer : register(u0);

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(DTid.x, DTid.y, DTid.z);
	float3 Position = posWS.Load(location).xyz;
	float specularExponent = posWS.Load(location).w;
	
	float3 Ambient = ambinetComponent.Load(location).xyz;
	
	backBuffer[DTid.xy] = float4(Ambient, 1.0f);
}