
#include <BasicShader.hlsli>

cbuffer ConstantBufferForCamera : register(b0)
{
	matrix view;		// ビュー変換行列
	matrix projection;	// プロジェクション変換行列
};

cbuffer ConstantBufferForPerFrame : register(b1)
{
	matrix world;	// ワールド変換行列
};

[maxvertexcount(3)]
void main(
	triangle GeometryShaderInput input[3],
	inout TriangleStream<PixelShaderInput> output)
{
	for (uint index = 0; index < 3; index++)
	{
		PixelShaderInput element;
		element.position = input[index].position;
		element.worldPosition = input[index].worldPosition;
		element.worldNormal = input[index].worldNormal;
		element.texCoord = input[index].texCoord;
		output.Append(element);
	}
}