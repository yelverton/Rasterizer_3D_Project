RWBuffer<float> particles : register(u0);

cbuffer dtTime : register(b0)
{
	float3 paddingDt;
	float dt;
};

[numthreads(1, 1, 1)]
void main(int3 threadId : SV_DispatchThreadID)
{
	particles[threadId.x * 3] = cos(dt + threadId.x);
	particles[threadId.x * 3 + 2] = sin(dt + threadId.x);
}