//#include "BasicShader.hlsli"
//
//VSOutput main(VSInput input)
//{
//	VSOutput output;
//	//// WVP•ÏŠ·
//	output.position = mul(input.position, WorldViewProjection);
//	output.worldPosition = mul(input.position, World);
//	output.worldNormal = normalize(mul(input.normal, (float3x3)World));
//	output.texCoord = input.texCoord;
//
//	return output;
//}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}
