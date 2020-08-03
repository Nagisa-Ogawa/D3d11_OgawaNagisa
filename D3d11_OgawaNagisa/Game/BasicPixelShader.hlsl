#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// �~����
#define EPSILON 1e-6

// �􉽏��
struct GeometricContext {
	float3 worldPosition;	// �ʒu
	float3 worldNormal;		// �@��
	float3 worldViewDir;		// �����ւ̌���
};
// ���s����
struct DirectionalLight {
	float3 direction;	// �����̌���
	float3 color;		// ���̐F
};
// ���̂̏��
struct Material {
	float3 diffuseColor;		// ���ˌ��̂����g�U���˂ɂȂ銄��
	float3 specularColor;		// ���ˌ��̂������ʔ��˂ɂȂ銄��
	float specularRoughness;	// ���̕\�ʂ̑e��
};
// ���ˌ�(������������Ă����)
struct IncidentLight {
	float3 direction;	// ���˂��Ă������
	float3 color;		// ���̔g�� = �F
	bool visible;		// ������������͂����ǂ����̃t���O
};
// ���ˌ�
struct ReflectedLight {
	float3 directDiffuse;		// ���ڌ��̊g�U���ː���
	float3 directSpecular;		// ���ڌ��̋��ʔ��ː���
	float3 indirectDiffuse;		// �Ԑڌ��̊g�U���ː���
	float3 indirectSpecular;	// �Ԑڌ��̋��ʔ��ː���
};

// �g�U����BRDF(���K�������o�[�g���f��)
// fd = pd/��
float3 DiffuseBRDF(float3 diffuseColor) {
	return diffuseColor / PI;
}

// �t���l��(F��)
// (schlink)(v,h) = F0 + (1-F0)(1-v�Eh)^5
// specularColor = F0
// H = �n�[�t�x�N�g��
// V = �����x�N�g��
float3 F_Schlick(float3 specularColor, float3 H, float3 V) {
	return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

// �}�C�N���t�@�Z�b�g���z�֐�(D��)
// D_GGX(h) = a^2 / ��((n�Eh)^2(a^2 - 1) + 1)^2
// a = ���t�l�X(���̂̑e���j
// dotNH = �@���ƃn�[�t�x�N�g���̓���
float D_GGX(float a, float dotNH) {
	float a2 = a * a;
	float dotNH2 = dotNH * dotNH;
	float d = dotNH2 * (a2 - 1.0) + 1.0;
	return a2 / (PI * d * d);
}

// �􉽌�����(G��)
// Smith���f����Schlick�ߎ�
// k = a / 2
// G Schlick(v) = n�Ev / (n�Ev)(1 - k) + k
// G Smith(l,v,h) = G Schlick(l)G Schlick(v)
// 
// a = ���t�l�X
// dotNV = �@���Ǝ����x�N�g���̓���
// dotNL = �@���ƃ��C�g�x�N�g���̓���
float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL) {
	float k = a * a * 0.5 + EPSILON;
	float g_Schlick_l = dotNL / (dotNL * (1.0 - k) + k);
	float g_Schlick_v = dotNV / (dotNV * (1.0 - k) + k);
	return g_Schlick_l * g_Schlick_v;
}

// Cook-Torrance���f��
// f(r,s) = DGF / 4(n�El)(n�Ev)
// 
// directLight = ���ˌ�
// geometry = ���̂̊􉽏��
// specularColor = ���ʔ��˗�
// roughnessFactor = ���t�l�X(���̂̑e��)
float3 SpecularBRDF
(IncidentLight directLight, GeometricContext geometry,
	float3 specularColor, float roughnessFactor) {

	float3 N = geometry.worldNormal;		// �@��
	float3 V = geometry.worldViewDir;	// �����x�N�g��
	float3 L = directLight.direction;	// ���C�g�x�N�g��

	float dotNL = saturate(dot(N, L));	// �@���ƃ��C�g�x�N�g���̓���
	float dotNV = saturate(dot(N, V));	// �@���Ǝ����x�N�g���̓���
		
	float3 H = normalize(L + V);		// �n�[�t�x�N�g��
	float dotNH = saturate(dot(N, H));	// �@���ƃn�[�t�x�N�g���̓���
	float dotVH = saturate(dot(V, H));	// �����x�N�g���ƃn�[�t�x�N�g���̓���
	float dotLV = saturate(dot(L, V));	// ���C�g�x�N�g���Ǝ����x�N�g���̓���
	float a = roughnessFactor * roughnessFactor;	// ���t�l�X�̓��

	// D��(GGX)
	float D = D_GGX(a, dotNH);
	// G��(Smith���f����Schlick�ߎ�)
	float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
	// F��(Schlick�̋ߎ���)
	float3 F = F_Schlick(specularColor, V, H);
	// f(r, s) = DGF / 4(n�El)(n�Ev)
	return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

float4 main(PSInput input) : SV_TARGET
{

	// ���̂̊􉽏��
	GeometricContext geometry;
	// ���[���h���W�n�̃|�W�V����
	geometry.worldPosition = input.worldPosition.xyz;
	// ���K���������[���h���W�n�̖@��
	geometry.worldNormal = normalize(input.worldNormal);
	// ���K���������[���h���W�n�̎����x�N�g��(���̂���J�����ւ�)
	geometry.worldViewDir = normalize(viewPosition.xyz - input.worldPosition.xyz);

	// ���̕\�ʂ̍ގ�
	Material material;
	// ���������������ݒ肷��p�����[�^ = ���ʔ��˗�
	float metallic = 0.0f;
	// ���̕\�ʂ̑e��
	float roughness = 0.0f;
	/*�g�����Ƃ̔��˔\ = �F���Ƃ̔��˔\*/
	float3 albedo = float3(0.0f, 0.0f, 0.0f);
	metallic = materialTexture.roughness_metallic.y;
	albedo = materialTexture.albedo.xyz;
	roughness = materialTexture.roughness_metallic.x;

	// albedo�`float3(0.0,0.0,0.0)��0�`1�Ƃ���������metallic����
	material.diffuseColor = lerp(albedo, float3(0.0,0.0,0.0), metallic);
	// float3(0.04,0.04,0.04)�`albedo��0�`1�Ƃ���������metallic����
	material.specularColor = lerp(float3(0.04,0.04,0.04), albedo, metallic);
	material.specularRoughness = roughness;

	// ���ˌ�
	ReflectedLight reflectedLight;
	// ������
	reflectedLight.directDiffuse= float3(0.0, 0.0, 0.0);
	reflectedLight.directSpecular = float3(0.0, 0.0, 0.0);
	reflectedLight.indirectDiffuse = float3(0.0, 0.0, 0.0);
	reflectedLight.indirectSpecular = float3(0.0, 0.0, 0.0);

	// 	���ȕ��ː���
	float3 emissive = float3(0.0,0.0,0.0);
	float opacity = 1.0;


	// ���s����
	DirectionalLight directionalLight;
	// ���s�����̌��������[���h���W�n�ɂ���
	directionalLight.direction = constantDirectionalLight.direction;
	directionalLight.color = constantDirectionalLight.color.xyz;

	// ���ˌ��ɕ��s�������Z�b�g
	IncidentLight incidentLight;
	incidentLight.direction = directionalLight.direction;
	incidentLight.color = directionalLight.color;

	// �R�T�C�������v�Z
	float dotNL = saturate(dot(geometry.worldNormal, incidentLight.direction));
	// ���ˋP�x�ƃR�T�C�������|���ĕ��ˏƓx�ɕϊ�
	float3 irradiance = dotNL * incidentLight.color;
	// irradiance�Ƀ΂��|���Ē���
	irradiance *= PI;

	// �����_�����O���������BRDF*���ˏƓx(���ˋP�x*�R�T�C����)
	// �g�U���ː���
	reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
	// ���ʔ��ː���
	reflectedLight.directSpecular += irradiance * SpecularBRDF(incidentLight, geometry, material.specularColor, material.specularRoughness);

	// ���ˋP�x = ���ȕ��ˁ@+ ���ː���(���ڌ��̊g�U���ː��� + ���ڌ��̋��ʔ��ː��� + �Ԑڌ��̊g�U���ː��� + �Ԑڌ��̋��ʔ��ː���)
	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz*outgoingLight, texel.w*opacity);
}

