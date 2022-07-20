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

cbuffer cam : register(b0)
{
	float3 cameraPosition;
	float padding0;
};

HS_CONTROL_DATA_OUTPUT ConstantPatchFunction(InputPatch<Vertex, 3> inputPatch, uint PatchID : SV_PrimitiveID)
{
	HS_CONTROL_DATA_OUTPUT Output;
	Vertex input;
	
	float3 getDistance;
	float3 pos = (inputPatch[0].posWS + inputPatch[1].posWS + inputPatch[2].posWS) / 3;
	
	getDistance.x = cameraPosition.x - pos.x;
	getDistance.y = cameraPosition.y - pos.y;
	getDistance.z = cameraPosition.z - pos.z;
	
	float camDistance = sqrt(getDistance.x * getDistance.x + getDistance.y * getDistance.y + getDistance.z * getDistance.z);
	if (camDistance > 10.0f)
		camDistance = 10.0f;
	
	if (camDistance < 1.0f)
		camDistance = 1.0f;
	
	camDistance -= 10.0f;
	
	camDistance = abs(camDistance);
	
	if (camDistance < 1.0f)
		camDistance = 1.0f;
		
	for (int i = 0; i < 3; ++i)
		Output.edgeTessFactor[i] = camDistance;
	
	Output.insideTessFactor = camDistance;

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantPatchFunction")]
Vertex perPatchFunction(InputPatch<Vertex, 3> inputPatch, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
	return inputPatch[i];
}

