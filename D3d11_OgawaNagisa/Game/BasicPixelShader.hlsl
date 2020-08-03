#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// 円周率
#define EPSILON 1e-6

// 幾何情報
struct GeometricContext {
	float3 worldPosition;	// 位置
	float3 worldNormal;		// 法線
	float3 worldViewDir;		// 視線への向き
};
// 平行光源
struct DirectionalLight {
	float3 direction;	// 光源の向き
	float3 color;		// 光の色
};
// 物体の情報
struct Material {
	float3 diffuseColor;		// 入射光のうち拡散反射になる割合
	float3 specularColor;		// 入射光のうち鏡面反射になる割合
	float specularRoughness;	// 物体表面の粗さ
};
// 入射光(光源から入ってくる光)
struct IncidentLight {
	float3 direction;	// 入射してくる向き
	float3 color;		// 光の波長 = 色
	bool visible;		// 減衰する光が届くかどうかのフラグ
};
// 反射光
struct ReflectedLight {
	float3 directDiffuse;		// 直接光の拡散反射成分
	float3 directSpecular;		// 直接光の鏡面反射成分
	float3 indirectDiffuse;		// 間接光の拡散反射成分
	float3 indirectSpecular;	// 間接光の鏡面反射成分
};

// 拡散反射BRDF(正規化ランバートモデル)
// fd = pd/π
float3 DiffuseBRDF(float3 diffuseColor) {
	return diffuseColor / PI;
}

// フレネル(F項)
// (schlink)(v,h) = F0 + (1-F0)(1-v・h)^5
// specularColor = F0
// H = ハーフベクトル
// V = 視線ベクトル
float3 F_Schlick(float3 specularColor, float3 H, float3 V) {
	return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

// マイクロファセット分布関数(D項)
// D_GGX(h) = a^2 / π((n・h)^2(a^2 - 1) + 1)^2
// a = ラフネス(物体の粗さ）
// dotNH = 法線とハーフベクトルの内積
float D_GGX(float a, float dotNH) {
	float a2 = a * a;
	float dotNH2 = dotNH * dotNH;
	float d = dotNH2 * (a2 - 1.0) + 1.0;
	return a2 / (PI * d * d);
}

// 幾何減衰項(G項)
// SmithモデルのSchlick近似
// k = a / 2
// G Schlick(v) = n・v / (n・v)(1 - k) + k
// G Smith(l,v,h) = G Schlick(l)G Schlick(v)
// 
// a = ラフネス
// dotNV = 法線と視線ベクトルの内積
// dotNL = 法線とライトベクトルの内積
float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL) {
	float k = a * a * 0.5 + EPSILON;
	float g_Schlick_l = dotNL / (dotNL * (1.0 - k) + k);
	float g_Schlick_v = dotNV / (dotNV * (1.0 - k) + k);
	return g_Schlick_l * g_Schlick_v;
}

// Cook-Torranceモデル
// f(r,s) = DGF / 4(n・l)(n・v)
// 
// directLight = 入射光
// geometry = 物体の幾何情報
// specularColor = 鏡面反射率
// roughnessFactor = ラフネス(物体の粗さ)
float3 SpecularBRDF
(IncidentLight directLight, GeometricContext geometry,
	float3 specularColor, float roughnessFactor) {

	float3 N = geometry.worldNormal;		// 法線
	float3 V = geometry.worldViewDir;	// 視線ベクトル
	float3 L = directLight.direction;	// ライトベクトル

	float dotNL = saturate(dot(N, L));	// 法線とライトベクトルの内積
	float dotNV = saturate(dot(N, V));	// 法線と視線ベクトルの内積
		
	float3 H = normalize(L + V);		// ハーフベクトル
	float dotNH = saturate(dot(N, H));	// 法線とハーフベクトルの内積
	float dotVH = saturate(dot(V, H));	// 視線ベクトルとハーフベクトルの内積
	float dotLV = saturate(dot(L, V));	// ライトベクトルと視線ベクトルの内積
	float a = roughnessFactor * roughnessFactor;	// ラフネスの二乗

	// D項(GGX)
	float D = D_GGX(a, dotNH);
	// G項(SmithモデルのSchlick近似)
	float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
	// F項(Schlickの近似式)
	float3 F = F_Schlick(specularColor, V, H);
	// f(r, s) = DGF / 4(n・l)(n・v)
	return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

float4 main(PSInput input) : SV_TARGET
{

	// 物体の幾何情報
	GeometricContext geometry;
	// ワールド座標系のポジション
	geometry.worldPosition = input.worldPosition.xyz;
	// 正規化したワールド座標系の法線
	geometry.worldNormal = normalize(input.worldNormal);
	// 正規化したワールド座標系の視線ベクトル(物体からカメラへの)
	geometry.worldViewDir = normalize(viewPosition.xyz - input.worldPosition.xyz);

	// 物体表面の材質
	Material material;
	// 金属か非金属かを設定するパラメータ = 鏡面反射率
	float metallic = 0.0f;
	// 物体表面の粗さ
	float roughness = 0.0f;
	/*波長ごとの反射能 = 色ごとの反射能*/
	float3 albedo = float3(0.0f, 0.0f, 0.0f);
	metallic = materialTexture.roughness_metallic.y;
	albedo = materialTexture.albedo.xyz;
	roughness = materialTexture.roughness_metallic.x;

	// albedo～float3(0.0,0.0,0.0)を0～1としたうちのmetallic部分
	material.diffuseColor = lerp(albedo, float3(0.0,0.0,0.0), metallic);
	// float3(0.04,0.04,0.04)～albedoを0～1としたうちのmetallic部分
	material.specularColor = lerp(float3(0.04,0.04,0.04), albedo, metallic);
	material.specularRoughness = roughness;

	// 反射光
	ReflectedLight reflectedLight;
	// 初期化
	reflectedLight.directDiffuse= float3(0.0, 0.0, 0.0);
	reflectedLight.directSpecular = float3(0.0, 0.0, 0.0);
	reflectedLight.indirectDiffuse = float3(0.0, 0.0, 0.0);
	reflectedLight.indirectSpecular = float3(0.0, 0.0, 0.0);

	// 	自己放射成分
	float3 emissive = float3(0.0,0.0,0.0);
	float opacity = 1.0;


	// 平行光源
	DirectionalLight directionalLight;
	// 平行光源の向きをワールド座標系にする
	directionalLight.direction = constantDirectionalLight.direction;
	directionalLight.color = constantDirectionalLight.color.xyz;

	// 入射光に平行光源をセット
	IncidentLight incidentLight;
	incidentLight.direction = directionalLight.direction;
	incidentLight.color = directionalLight.color;

	// コサイン項を計算
	float dotNL = saturate(dot(geometry.worldNormal, incidentLight.direction));
	// 放射輝度とコサイン項を掛けて放射照度に変換
	float3 irradiance = dotNL * incidentLight.color;
	// irradianceにπを掛けて調整
	irradiance *= PI;

	// レンダリング方程式よりBRDF*放射照度(放射輝度*コサイン項)
	// 拡散反射成分
	reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
	// 鏡面反射成分
	reflectedLight.directSpecular += irradiance * SpecularBRDF(incidentLight, geometry, material.specularColor, material.specularRoughness);

	// 放射輝度 = 自己放射　+ 反射成分(直接光の拡散反射成分 + 直接光の鏡面反射成分 + 間接光の拡散反射成分 + 間接光の鏡面反射成分)
	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz*outgoingLight, texel.w*opacity);
}

