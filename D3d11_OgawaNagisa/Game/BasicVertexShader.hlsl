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

GeometryShaderInput main(VertexShaderInput input)
{
	GeometryShaderInput output;
	// �ʒu���W���v���W�F�N�V������Ԃ֕ϊ�
	float4 worldPosition = mul(world, input.position);
	float4 viewPosition = mul(view, worldPosition);
	output.position = mul(projection, viewPosition);
	// �ʒu���W�����[���h��Ԃ֕ϊ�
	output.worldPosition = mul(world, input.position).xyz;
	// �@���x�N�g�����r���[��Ԃ֕ϊ�
	float3 worldNormal = mul((float3x3)world, input.normal);
	float4 viewNormal = mul(view, float4(worldNormal.xyz, 0.0f));
	output.worldNormal = normalize(worldNormal);
	output.texCoord = input.texCoord;
	return output;
}
