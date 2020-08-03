#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// �~����
#define EPSILON 1e-6

// �􉽏��
struct GeometricContext {
	float3 position;	// �ʒu
	float3 normal;		// �@��
	float3 viewDir;		// �����ւ̌���
};

// ���ˌ�(������������Ă����)
struct IncidentLight {
	float3 direction;	// ���˂��Ă������
	float3 color;		// ���̔g�� = �F
	bool visible;		// ������������͂����ǂ����̃t���O
};

// ���s����
struct DirectionalLight {
	float3 direction;	// �����̌���
	float3 color;		// ���̐F
};

// �_����
struct PointLight {
	float3 position;	// �����̈ʒu
	float3 color;		// ���̐F
	float distance;		// ������������͂�����
	float decay;		// ������
};

// �X�|�b�g���C�g
struct SpotLight {
	float3 position;	// �����̈ʒu
	float3 direction;	// �����̌���
	float3 color;		// ���̐F
	float distance;		// ������������͂�����
	float decay;		// ������
	float coneCos;		// ��������o�˂�������̕�
	float penumbraCos;	// ��������o�˂�������̌�����
};

// ���̂̏��
struct Material {
	float3 diffuseColor;		// ���ˌ��̂����g�U���˂ɂȂ銄��
	float3 specularColor;		// ���ˌ��̂������ʔ��˂ɂȂ銄��
	float specularRoughness;	// ���̕\�ʂ̑e��
};

// ���ˌ�
struct ReflectedLight {
	float3 directDiffuse;		// ���ڌ��̊g�U���ː���
	float3 directSpecular;		// ���ڌ��̋��ʔ��ː���
	float3 indirectDiffuse;		// �Ԑڌ��̊g�U���ː���
	float3 indirectSpecular;	// �Ԑڌ��̋��ʔ��ː���
};

// ������������͂����ǂ����`�F�b�N����֐�
// lightDistance = ���̂�������܂ł̋���
// cutoffDistance = ������������͂�����
bool CheckLightRange(float lightDistance, float cutoffDistance) {
	// ������������͂�������0
	// ��������, ���̂�������܂ł̋�����������������͂������̂ق�������
	// �ǂ��炩��true�Ȃ�true
	return any(bool2(cutoffDistance == 0.0, lightDistance < cutoffDistance));
}

// ��������̌��������v�Z����֐�
// lightDistance = ���̂�������܂ł̋���
// cutoffDistance = ������������͂�����
// decayExponent = �����̌�����
float DecayFromLightSource
(float lightDistance, float cutoffDistance, float decayExponent) {
	// ��������0���傫���Ȃ�
	if (decayExponent > 0.0) {
		// (-���̂�������܂ł̋��� / ������������͂����� + 1.0)�̌����̌����� ��
		return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
	}

	// ��������0�Ȃ�1.0�ɂ���
	return 1.0;
}

// ���s��������̓��ˌ��̏����擾����֐�
// directionalLight = ���s����
// geometry = ���̂̊􉽏��
// directLight = ���ˌ�(�Q�Ɠn��)
void getDirectionalDirectLightIrradiance
(DirectionalLight directionalLight,
	GeometricContext geometry, out IncidentLight directLight) {
	// ���ˌ��̐F�𕽍s�����̐F�ɂ���
	directLight.color = directionalLight.color;
	// ���ˌ��̌����𕽍s�����̌����ɂ���
	directLight.direction = directionalLight.direction;
	directLight.visible = true;
}

// �_��������̓��ˌ��̏����擾����֐�
// pointLight = ���s����
// geometry = ���̂̊􉽏��
// directLight = ���ˌ�(�Q�Ɠn��)
void getPointDirectLightIrradiance
(PointLight pointLight, GeometricContext geometry, out IncidentLight directLight) {
	// �������畨�̂܂ł̋���
	float3 L = pointLight.position - geometry.position;
	// ���K���������̕���
	directLight.direction = normalize(L);

	// ���̒���
	float lightDistance = length(L);
	// �����͂��Ă��邩�`�F�b�N
	if (CheckLightRange(lightDistance, pointLight.distance)) {
		directLight.color = pointLight.color;
		// �������v�Z
		directLight.color *= DecayFromLightSource(lightDistance, pointLight.distance, pointLight.decay);
		directLight.visible = true;
	}
	// �͂��Ă��Ȃ��Ȃ�v�Z�Ɋ܂܂Ȃ�
	else {
		directLight.color = float3(0.0f,0.0f,0.0f);
		directLight.visible = false;
	}
}

void getSpotDirectLightIrradiance
(SpotLight spotLight, GeometricContext geometry, out IncidentLight directLight) {
	float3 L = spotLight.position - geometry.position;
	directLight.direction = normalize(L);

	float lightDistance = length(L);
	float angleCos = dot(directLight.direction, spotLight.direction);

	if (all(bool2(angleCos > spotLight.coneCos, CheckLightRange(lightDistance, spotLight.distance)))) {
		float spotEffect = smoothstep(spotLight.coneCos, spotLight.penumbraCos, angleCos);
		directLight.color = spotLight.color;
		directLight.color *= spotEffect * DecayFromLightSource(lightDistance, spotLight.distance, spotLight.decay);
		directLight.visible = true;
	}
	else {
		directLight.color = float3(0.0f, 0.0f, 0.0f);
		directLight.visible = false;
	}
}

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

	float3 N = geometry.normal;		// �@��
	float3 V = geometry.viewDir;	// �����x�N�g��
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

// RenderEquations(�����_�����O������)
// �����_�����O���������v�Z����֐�
void RenderEquations_Direct
(IncidentLight directLight, GeometricContext geometry,
	Material material, inout ReflectedLight reflectedLight) {

	// �R�T�C�������v�Z
	float dotNL = saturate(dot(geometry.normal, directLight.direction));
	// ���ˋP�x�ƃR�T�C�������|���ĕ��ˏƓx�ɕϊ�
	float3 irradiance = dotNL * directLight.color;

	// punctuallight�Ƀ΂��|���Ē���
	irradiance *= PI;

	// �����_�����O���������BRDF*���ˋP�x*�R�T�C����
	// �g�U���ː���
	reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
	// ���ʔ��ː���
	reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

float4 main(PSInput input) : SV_TARGET
{
	{
		//float4 worldPosition = input.worldPosition;
		//// �ʂ���������w�����K���x�N�g��L(���s����(w=0)�Ɠ_����(w=1))
		//float3 light = normalize(LightPosition.xyz - LightPosition.w * worldPosition.xyz);

		//// ���[���h��ԏ�̖@���x�N�g��N
		//float3 worldNormal = input.worldNormal;
		//// �g�U����
		//float diffuse = max	// �傫���ق��̒l���g�p
		//(
		//	dot(light, worldNormal),	// �����Ɩ@���x�N�g���̓���
		//	0							// �}�C�i�X�ȉ��Ȃ�0
		//);
		//float3 diffuseColor = diffuse * MaterialDiffuse.xyz;

		//// ���ʔ���

		//// �����˕���
		//// �����˕����̃x�N�g�� = 2 * �@���x�N�g�� * �@���x�N�g���Ɩʂ���������w�����K���x�N�g���̓��� - �ʂ���������w�����K���x�N�g��
		//float3 reflect = 2 * input.worldNormal * dot(input.worldNormal, light) - light;
		//// ���_����
		//// ���_�����ւ̃x�N�g�� = ���K������(�J�����̈ʒu���W - ���[���h��ԏ�̈ʒu���W).xyz
		//float3 viewDir = normalize(ViewPosition - input.worldPosition).xyz;
		//// specular = (clamp(�����˕����Ǝ��_�����̓���))��MaterialSpecularPower(���ʂ̋���)��
		//float specular = pow(saturate(dot(reflect, viewDir)), MaterialSpecularPower);
		//// specularColor = specular * ���ʔ��˃J���[
		//float3 specularColor = specular * MaterialSpecularColor.xyz;

		//float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);

		//// �g�U���˃J���[ + ���ʔ��˃J���[
		//return float4(texel.xyz * diffuseColor + specularColor, MaterialDiffuse.w * texel.w);
	}

	{
		//// ���[���h��ԏ�̍��W
		//float4 worldPosition = input.worldPosition;
		//// ���[���h��ԏ�̖@���x�N�g��N
		//float3 worldNormal = input.worldNormal;
		//// ���_����
		//// ���_�����ւ̃x�N�g�� = ���K������(�J�����̈ʒu���W - ���[���h��ԏ�̈ʒu���W).xyz
		//float3 viewDir = normalize(eyePosition.xyz - worldPosition.xyz);
		//// �ʂ���������w�����K���x�N�g��L(���s����(w=0)�Ɠ_����(w=1))
		//float3 l = light.lightPosition.xyz - light.lightPosition.w * worldPosition.xyz;
		//float3 halfVec = normalize(l + viewDir);
		////��������
		//l = normalize(l);
		////// �����˕���
		////// �����˕����̃x�N�g�� = 2 * �@���x�N�g�� * �@���x�N�g���Ɩʂ���������w�����K���x�N�g���̓��� - �ʂ���������w�����K���x�N�g��
		//float3 reflect = 2.0 * worldNormal * dot(worldNormal, l) - l;

		//// �����ʖ@�����z�֐�
		//// GGX���g�p
		//// D(h) = a^2 / ��((n�Eh)^2(a2-1)+1)^2
		//float a = material.roughness.x;
		//float D = a*a / PI * pow(dot(worldNormal, halfVec), 2.0f) * pow((a*a - 1) + 1, 2.0f);
		//		

		//float3 iA;
		//float3 iD;
		//float3 iS;
		//// ������
		//iA = material.materialAmbient.xyz * ambient.xyz;
		//// �g�U����
		//// �������w�����K���x�N�g���Ɩ@���x�N�g���Ƃ̓��� * ���ʔ��˃J���[
		//iD = (max(dot(l, worldNormal), 0) * (1.0f / PI)) * material.materialDiffuse.xyz * light.lightDiffuse.xyz;
		//// ���ʔ���
		//// specular = (clamp(�����˕����Ǝ��_�����̓���))��MaterialSpecularPower(���ʂ̋���)�� * ���ʔ��˃J���[
		//iS = pow(saturate(dot(reflect, viewDir)), material.materialSpecular.w) * material.materialSpecular.xyz * light.lightSpecular.xyz;

		//return float4(saturate(iA + iD + iS), 1.0);
	}

	// ���̂̊􉽏��
	GeometricContext geometry;
	// - (�J�����̍��W - ���[���h��ԏ�̈ʒu���W) = �J�������W�n�̈ʒu���W
	geometry.position = -(viewPosition.xyz - input.worldPosition.xyz);
	// ���K���������[���h��ԏ�̖@��
	geometry.normal = normalize(input.worldNormal);
	// ���K�����������x�N�g�� = �J�����̍��W - ���[���h��ԏ�̈ʒu���W
	geometry.viewDir = normalize(viewPosition - input.worldPosition).xyz;

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

	// 
	float3 emissive = float3(0.0,0.0,0.0);
	// ���ȕ��ː���(�萔)
	float opacity = 1.0;

	// ���ˌ�
	IncidentLight directLight;

	// ���s����
	DirectionalLight directionalLight;
	// �_����
	PointLight pointLight;

	//�e�����̏�����
	directionalLight.direction = constantDirectionalLight.direction.xyz;
	directionalLight.color = constantDirectionalLight.color.xyz;

	pointLight.position = constantPointLight.position.xyz;
	pointLight.color = constantPointLight.color.xyz;
	pointLight.distance = constantPointLight.distance_decay.x;
	pointLight.decay = constantPointLight.distance_decay.y;

	// point light
	// ���C�g�̐������v�Z
	// for (int i = 0; i < LIGHT_MAX; ++i) {
		// if (i >= numPointLights) break;
	// ���s�����̏��Ɗ􉽏�񂩂���ˌ��̃p�����[�^��ݒ�
	getPointDirectLightIrradiance(pointLight, geometry, directLight);
	// �����͂��Ȃ�
	if (directLight.visible) {
		// �����_�����O����������v�Z
		RenderEquations_Direct(directLight, geometry, material, reflectedLight);
	}
	//}

	// spot light
	// for (int i = 0; i < LIGHT_MAX; ++i) {
		// if (i >= numSpotLights) break;
		//getSpotDirectLightIrradiance(spotLight, geometry, directLight);
		//if (directLight.visible) {
		//	RenderEquations_Direct(directLight, geometry, material, reflectedLight);
		//}
	//}

	// directional light
	//for (int i = 0; i < LIGHT_MAX; ++i) {
		// if (i >= numDirectionalLights) break;
	getDirectionalDirectLightIrradiance(directionalLight, geometry, directLight);
	// �����_�����O����������v�Z
	RenderEquations_Direct(directLight, geometry, material, reflectedLight);
	// }

	// ���ˋP�x = ���ȕ��ˁ@+ ���ː���(���ڌ��̊g�U���ː��� + ���ڌ��̋��ʔ��ː��� + �Ԑڌ��̊g�U���ː��� + �Ԑڌ��̋��ʔ��ː���)
	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz*outgoingLight, texel.w*opacity);
	// return float4(outgoingLight, opacity);
}

