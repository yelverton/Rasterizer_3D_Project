struct DomainShaderOutput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : normal;
	float3 posWS : WS_POSITION;
	float4 posLight : LIGHTPOS;
};

struct HS_CONTROL_DATA_OUTPUT
{
	float edgeTessFactor[3] : SV_TessFactor;
	float insideTessFactor : SV_InsideTessFactor;
};

struct Vertex
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : normal;
	float3 posWS : WS_POSITION;
	float4 posLight : LIGHTPOS;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DomainShaderOutput main(HS_CONTROL_DATA_OUTPUT input, float3 uvw : SV_DomainLocation,
						const OutputPatch<Vertex, 3> patch)
{
	DomainShaderOutput Output;

	Output.position = patch[0].position * uvw.x + patch[1].position * uvw.y + patch[2].position * uvw.z;
	Output.uv = patch[0].uv * uvw.x + patch[1].uv * uvw.y + patch[2].uv * uvw.z;
	Output.normal = patch[0].normal * uvw.x + patch[1].normal * uvw.y + patch[2].normal * uvw.z;
	Output.posWS = patch[0].posWS * uvw.x + patch[1].posWS * uvw.y + patch[2].posWS * uvw.z;
	Output.posLight = patch[0].posLight * uvw.x + patch[1].posLight * uvw.y + patch[2].posLight * uvw.z;
	
	return Output;
}
