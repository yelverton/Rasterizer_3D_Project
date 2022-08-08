cbuffer Matrices : register(b0)
{
	float3 forwardVec;
	float padding0;
	float3 upVec;
	float padding1;
}

cbuffer Matrices : register(b1)
{
	matrix view;
	matrix projection;
}

struct GSOutput
{
	float4 pos : SV_POSITION;
};

struct VertexShaderOutput
{
	float4 pos : SV_Position;
};

struct GeometryShaderOutput
{
	float4 pos : SV_Position;
};

[maxvertexcount(4)] // create a QUAD
void main(point float4 input[1] : SV_Position, inout TriangleStream<GeometryShaderOutput> outputStream)
{
	const static float SIZE = 0.25f;
	
	matrix viewProj = mul(view, projection);
	
	float3 right = cross(forwardVec, upVec);
	
	float3 topLeft = input[0].xyz - right * SIZE + upVec * SIZE;
	float3 topRight = input[0].xyz + right * SIZE + upVec * SIZE;
	float3 bottomRight = input[0].xyz + right * SIZE - upVec * SIZE;
	float3 bottomLeft = input[0].xyz - right * SIZE - upVec * SIZE;
	
	GeometryShaderOutput outputs[4] =
	{
		mul(float4(bottomLeft, 1.0f), viewProj)
		, mul(float4(bottomRight, 1.0f), viewProj)
		, mul(float4(topLeft, 1.0f), viewProj)
		, mul(float4(topRight, 1.0f), viewProj)
	};
	
	outputStream.Append(outputs[0]);
	outputStream.Append(outputs[1]);
	outputStream.Append(outputs[2]);
	outputStream.Append(outputs[3]);
};