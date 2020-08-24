
//// ���s�����p�\����
//struct ConstantDirectionalLight {
//	float4 direction;
//	float4 color;
//};
//
//// ���̕\�ʂ̎����p�\����
//struct ConstantMaterial {
//	float3 baseColor;
//	float metallic;
//	float3 specular;
//	float roughness;
//};
//
//// �萔�o�b�t�@�[
//cbuffer ConstantBuffer
//{
//	// ���[���h�ϊ��s��
//	float4x4 World;
//	// �r���[�ϊ��s��
//	float4x4 View;
//	// �����ˉe�ϊ��s��
//	float4x4 Projection;
//	// WVP�s��
//	float4x4 WorldViewProjection;
//	//���_���W
//	float4 viewPosition;
//	// ���s�������
//	ConstantDirectionalLight constantDirectionalLight;
//	// ���̕\�ʂ̎����̏��
//	ConstantMaterial constantMaterial;
//}
//
//// ���_�V�F�[�_�\�ւ̓���
//struct VSInput
//{
//	float4 position : POSITION;
//	float3 normal : NORMAL;
//	float2 texCoord : TEXCOORD;
//};
//
//// ���_�V�F�[�_�[����̏o��
//struct VSOutput
//{
//	float4 position : SV_POSITION;
//	float4 worldPosition : POSITION;
//	float3 worldNormal : NORMAL;
//	float2 texCoord : TEXCOORD;
//};
//
//// �W�I���g���[�V�F�[�_�[�ւ̓���
//typedef VSOutput GSInput;
//
//// �W�I���g���[�V�F�[�_�[����̏o��
//struct GSOutput
//{
//	float4 position : SV_POSITION;
//	float4 worldPosition : POSITION;
//	float3 worldNormal : NORMAL;
//	float2 texCoord : TEXCOORD;
//};
//
//// �s�N�Z���V�F�[�_�\�ւ̓���
//typedef GSOutput PSInput;
// ���_�V�F�[�_�[�̓���
struct VertexShaderInput
{
	float4 position : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord  : TEXCOORD;
};

// �W�I���g���[ �V�F�[�_�[�̓���
struct GeometryShaderInput
{
	float4 position      : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 worldNormal   : NORMAL;
	float2 texCoord  : TEXCOORD;
};

// �s�N�Z�� �V�F�[�_�[�̓���
struct PixelShaderInput
{
	float4 position      : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 worldNormal   : NORMAL;
	float2 texCoord  : TEXCOORD;
};
