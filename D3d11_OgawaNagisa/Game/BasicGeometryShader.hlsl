//#include "BasicShader.hlsli"
//
//[maxvertexcount(3)]
//void main(
//	triangle VSOutput input[3],
//	inout TriangleStream< GSOutput > output)
//{
//	[unroll]
//	for (uint i = 0; i < 3; i++)
//	{
//		GSOutput element;
//		element.position = input[i].position;
//		element.worldPosition = input[i].worldPosition;
//		element.worldNormal = input[i].worldNormal;
//		element.texCoord = input[i].texCoord;
//		output.Append(element);
//	}
//}

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i];
		output.Append(element);
	}
}