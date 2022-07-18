Texture2D<float4> posWS : register(t0);

RWTexture2D<unorm float4> backBuffer : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(DTid.x, DTid.y, DTid.z);
	float3 pos = posWS.Load(location).xyz;
	
	backBuffer[DTid.xy] = float4(pos, 1.0f);

}