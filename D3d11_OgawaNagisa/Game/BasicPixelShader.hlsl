#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// �~����

// ���̂̏��
struct Material {
	float3 diffuseRatioColor;		// ���ˌ��̂����g�U���˂ɂȂ銄��
	float3 specularRatioColor;		// ���ˌ��̂������ʔ��˂ɂȂ銄��
	float roughness;	// ���̕\�ʂ̑e��
};

// ���ˌ�(������������Ă����)
struct IncidentLight {
	float4 direction;	// ���˂��Ă������
	float4 color;		// ���̔g�� = �F
};

// �g�U����BRDF(���K�������o�[�g���f��)
// fd = pd/��
float3 DiffuseBRDF(float3 diffuseColor) {
	return diffuseColor / PI;
}

// �t���l��(F��)
// (schlink)(v,h) = F0 + (1-F0)(1-v�Eh)^5
// specularRatioColor = F0
float3 F_Schlick(float3 specularRatioColor, float3 halfVec, float3 ViewDir) {
	return (specularRatioColor + (1.0 - specularRatioColor) * pow(1.0 - saturate(dot(halfVec, ViewDir)), 5));
}

// �}�C�N���t�@�Z�b�g���z�֐�(D��)
// D_GGX(h) = a^2 / ��((n�Eh)^2(a^2 - 1) + 1)^2
// a = ���t�l�X(���̂̑e���j
float D_GGX(float a, float dotNH) {
	float x = dotNH * dotNH * (a * a - 1) + 1;
	return a * a / (PI * x * x);
}

// �􉽌�����(G��)
// Smith���f����Schlick�ߎ�
// k = a * ��2/PI
// G Schlick(v) = n�Ev / (n�Ev)(1 - k) + k
// G Smith(l,v,h) = G Schlick(l)G Schlick(v)
// a = ���t�l�X
float G_Smith_Schlick(float a, float dotNV, float dotNL) {
	float k = a * sqrt(2 / PI);
	float g_Schlick_l = dotNL / (dotNL * (1.0 - k) + k);
	float g_Schlick_v = dotNV / (dotNV * (1.0 - k) + k);
	return g_Schlick_l * g_Schlick_v;
}

// Cook-Torrance���f��
// f(r,s) = DGF / 4(n�El)(n�Ev)
float3 SpecularBRDF
(IncidentLight incidentLight, float3 worldNormal, float3 worldViewDir,
	float3 specularColor, float roughness) {

	float3 lightDirection = incidentLight.direction;	// ���C�g�x�N�g��
	float3 halfVec = normalize(lightDirection + worldViewDir);		// �n�[�t�x�N�g��

	float dotNL = saturate(dot(worldNormal,lightDirection ));	// �@���ƃ��C�g�x�N�g���̓���
	float dotNV = saturate(dot(worldNormal, worldViewDir));	// �@���Ǝ����x�N�g���̓���	
	float dotNH = saturate(dot(worldNormal, halfVec));	// �@���ƃn�[�t�x�N�g���̓���
	float dotVH = saturate(dot(worldViewDir, halfVec));	// �����x�N�g���ƃn�[�t�x�N�g���̓���
	float dotLV = saturate(dot(lightDirection, worldViewDir));	// ���C�g�x�N�g���Ǝ����x�N�g���̓���
	float a = roughness * roughness;	// ���t�l�X�̓��

	// D��(GGX)
	float D = D_GGX(a, dotNH);
	// G��(Smith���f����Schlick�ߎ�)
	float G = G_Smith_Schlick(a, dotNV, dotNL);
	// F��(Schlick�̋ߎ���)
	float3 F = F_Schlick(specularColor, worldViewDir, halfVec);
	// f(r, s) = DGF / 4(n�El)(n�Ev)
	return (F * (G * D)) / (4.0 * dotNL * dotNV);
}

float4 main(PSInput input) : SV_TARGET
{
	// ���[���h���W�n�̃|�W�V����
	float3 worldPosition = input.worldPosition.xyz;
	// ���K���������[���h���W�n�̖@��
	float3 worldNormal = normalize(input.worldNormal);
	// ���K���������[���h���W�n�̎����x�N�g��(���̂���J�����ւ�)
	float3 worldViewDir = normalize(viewPosition.xyz - input.worldPosition.xyz);

	// ���̕\�ʂ̍ގ�
	Material material;
	// ���������������ݒ肷��p�����[�^ = ���ʔ��˗�
	float metallic = 0.0f;
	// ���̕\�ʂ̑e��
	// ���������������ݒ肷��p�����[�^ =
	float roughness = 0.0f;
	/*�g�����Ƃ̔��˔\ = �F���Ƃ̔��˔\*/
	float3 albedo = float3(0.0f, 0.0f, 0.0f);
	metallic = materialTexture.roughness_metallic.y;
	albedo = materialTexture.albedo.xyz;
	roughness = materialTexture.roughness_metallic.x;

	// albedo�`float3(0.0,0.0,0.0)��0�`1�Ƃ���������metallic����
	material.diffuseRatioColor = lerp(albedo, float3(0.0,0.0,0.0), metallic);
	// float3(0.04,0.04,0.04)�`albedo��0�`1�Ƃ���������metallic����
	material.specularRatioColor = lerp(float3(0.04,0.04,0.04), albedo, metallic);
	material.roughness = roughness;

	float3 reflectedDiffuse= float3(0.0, 0.0, 0.0);
	float3 reflectedSpecular = float3(0.0, 0.0, 0.0);

	// 	���ȕ��ː���
	float3 emissive = float3(0.0,0.0,0.0);
	float opacity = 1.0;


	// ���ˌ��ɒ萔�o�b�t�@�[�̕��s�������Z�b�g
	IncidentLight incidentLight = constantDirectionalLight;

	// �R�T�C�������v�Z
	float dotNL = saturate(dot(worldNormal, incidentLight.direction));
	// ���ˋP�x�ƃR�T�C�������|���ĕ��ˏƓx�ɕϊ�
	float3 irradiance = dotNL * incidentLight.color;
	// irradiance�Ƀ΂��|���Ē���
	irradiance *= PI;

	// �����_�����O��������蔽�ː��� = BRDF * ���ˏƓx(���ˋP�x * �R�T�C����)
	// �g�U���ː���
	reflectedDiffuse += DiffuseBRDF(material.diffuseRatioColor) * irradiance;
	// ���ʔ��ː���
	reflectedSpecular += 
		SpecularBRDF(incidentLight, worldNormal, worldViewDir, material.specularRatioColor, material.roughness) * irradiance;
	// ���ˋP�x = ���ȕ��ˁ@+ ���ː���(���ڌ��̊g�U���ː��� + ���ڌ��̋��ʔ��ː���)
	float3 Radiance = emissive + reflectedDiffuse + reflectedSpecular;
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz* Radiance, texel.w*opacity);
	// return float4(outgoingLight, opacity);
}

