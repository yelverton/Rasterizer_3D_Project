struct PixelShaderInput
{
	float4 position : SV_POSITION;
};

struct vsOutPut
{
	float4 posWS : SV_TARGET0;
};

vsOutPut main(PixelShaderInput input) : SV_TARGET
{
	vsOutPut output;
	
	output.posWS = input.position;
	
	return output;
}