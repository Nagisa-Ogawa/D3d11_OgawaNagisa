
// ���s�����p�\����
struct ConstantDirectionalLight {
	float4 direction;
	float4 color;
};

// �_�����p�\����
struct ConstantPointLight {
	float4 position;
	float4 color;
	float4 distance_decay;
};

// ���̕\�ʂ̎����p�\����
struct MaterialTexture {
	float4 roughness_metallic;
	float4 albedo;
};

// �萔�o�b�t�@�[
cbuffer ConstantBuffer
{
	// ���[���h�ϊ��s��
	float4x4 World;
	// �r���[�ϊ��s��
	float4x4 View;
	// �����ˉe�ϊ��s��
	float4x4 Projection;
	// WVP�s��
	float4x4 WorldViewProjection;
	//���_���W
	float4 viewPosition;
	// ���s�������
	ConstantDirectionalLight constantDirectionalLight;
	// �_�������
	ConstantPointLight constantPointLight;
	// ���̕\�ʂ̎����̏��
	MaterialTexture materialTexture;
}

// ���_�V�F�[�_�\�ւ̓���
struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ���_�V�F�[�_�[����̏o��
struct VSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 worldNormal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// �W�I���g���[�V�F�[�_�[�ւ̓���
typedef VSOutput GSInput;

// �W�I���g���[�V�F�[�_�[����̏o��
struct GSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 worldNormal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// �s�N�Z���V�F�[�_�\�ւ̓���
typedef GSOutput PSInput;
