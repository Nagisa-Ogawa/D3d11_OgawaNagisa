
#include <BasicShader.hlsli>

cbuffer ConstantBufferForCamera : register(b0)
{
	matrix view;		// �r���[�ϊ��s��
	matrix projection;	// �v���W�F�N�V�����ϊ��s��
};

cbuffer ConstantBufferForPerFrame : register(b1)
{
	matrix world;	// ���[���h�ϊ��s��
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