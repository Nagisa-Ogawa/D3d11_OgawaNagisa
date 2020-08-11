//#include "BasicShader.hlsli"
//
//VSOutput main(VSInput input)
//{
//	VSOutput output;
//	//// WVP�ϊ�
//	output.position = mul(input.position, WorldViewProjection);
//	output.worldPosition = mul(input.position, World);
//	output.worldNormal = normalize(mul(input.normal, (float3x3)World));
//	output.texCoord = input.texCoord;
//
//	return output;
//}

cbuffer ConstantBufferForCamera : register(b0)
{
	matrix view;		// �r���[�ϊ��s��
	matrix projection;	// �v���W�F�N�V�����ϊ��s��
};

cbuffer ConstantBufferForPerFrame : register(b1)
{
	matrix world;	// ���[���h�ϊ��s��
};

float4 main(float4 position : POSITION) : SV_POSITION
{
	float4 worldPosition = mul(world, position);
	float4 viewPosition = mul(view, worldPosition);
	float4 projectionPosition = mul(projection, viewPosition);
	return projectionPosition;
}
