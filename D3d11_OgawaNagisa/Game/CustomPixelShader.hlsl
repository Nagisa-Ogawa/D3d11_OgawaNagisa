#include "BasicShader.hlsli"

// 平行光源用構造体
struct ConstantDirectionalLight {
	float4 direction;
	float4 color;
};

// 物体表面の質感用構造体
struct ConstantMaterial {
	float3 baseColor;
	float metallic;
	float3 specular;
	float roughness;
};


cbuffer ConstantBufferForLighting : register(b0)
{
	ConstantDirectionalLight constantDirectionalLight;
	ConstantMaterial constantMaterial;
	float4 viewPosition;
};


Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// 円周率

// 物体の情報
struct Material {
	float3 baseColor;		// 物体表面の色
	float metallic;		// 金属度合い
	float3 specular;		// 鏡面反射の調整値
	float roughness;		// 物体の粗さ
};

// 入射光(光源から入ってくる光)
struct IncidentLight {
	float4 direction;	// 入射してくる向き
	float4 color;		// 光の波長 = 色
};

// 拡散反射BRDF(正規化ランバートモデル)
// fd = pd/π
float3 DiffuseBRDF(float3 baseColor) {
	return baseColor / PI;
}

// フレネル(F項)
// (schlink)(v,h) = F0 + (1-F0)(1-v・h)^5
// specularRatioColor = F0
float3 F_Schlick(float3 specularRatioColor, float3 halfVec, float3 ViewDir) {
	return (specularRatioColor + (1.0 - specularRatioColor) * pow(1.0 - saturate(dot(halfVec, ViewDir)), 5));
}

// マイクロファセット分布関数(D項)
// D_GGX(h) = a^2 / π((n・h)^2(a^2 - 1) + 1)^2
// a = ラフネス(物体の粗さ）
float D_GGX(float a, float dotNH) {
	float x = dotNH * dotNH * (a * a - 1) + 1;
	return a * a / (PI * x * x);
}

// 幾何減衰項(G項)
// SmithモデルのSchlick近似
// k = a * √2/PI
// G Schlick(v) = n・v / (n・v)(1 - k) + k
// G Smith(l,v,h) = G Schlick(l)G Schlick(v)
// a = ラフネス
float G_Smith_Schlick(float a, float dotNV, float dotNL) {
	float k = a * sqrt(2 / PI);
	float g_Schlick_l = dotNL / (dotNL * (1.0 - k) + k);
	float g_Schlick_v = dotNV / (dotNV * (1.0 - k) + k);
	return g_Schlick_l * g_Schlick_v;
}

// Cook-Torranceモデル
// f(r,s) = DGF / 4(n・l)(n・v)
float3 SpecularBRDF
(IncidentLight incidentLight, float3 worldNormal, float3 worldViewDir, float3 specular, Material material) {

	float3 lightDirection = incidentLight.direction;	// ライトベクトル
	float3 halfVec = normalize(lightDirection + worldViewDir);		// ハーフベクトル

	float dotNL = saturate(dot(worldNormal,lightDirection ));	// 法線とライトベクトルの内積
	float dotNV = saturate(dot(worldNormal, worldViewDir));	// 法線と視線ベクトルの内積	
	float dotNH = saturate(dot(worldNormal, halfVec));	// 法線とハーフベクトルの内積
	float dotVH = saturate(dot(worldViewDir, halfVec));	// 視線ベクトルとハーフベクトルの内積
	float dotLV = saturate(dot(lightDirection, worldViewDir));	// ライトベクトルと視線ベクトルの内積
	float a = material.roughness * material.roughness;	// ラフネスの二乗

	// D項(GGX)
	float D = D_GGX(a, dotNH);
	// G項(SmithモデルのSchlick近似)
	float G = G_Smith_Schlick(a, dotNV, dotNL);
	// F項(Schlickの近似式)
	float3 F = F_Schlick(specular, worldViewDir, halfVec);
	// f(r, s) = DGF / 4(n・l)(n・v)
	return (F * (G * D)) / (4.0 * dotNL * dotNV);
}

float4 main(PixelShaderInput input) : SV_TARGET
{
	// ワールド座標系のポジション
	float3 worldPosition = input.worldPosition.xyz;
	// 正規化したワールド座標系の法線
	float3 worldNormal = normalize(input.worldNormal);
	// 正規化したワールド座標系の視線ベクトル(物体からカメラへの)
	float3 worldViewDir = normalize(viewPosition.xyz - input.worldPosition.xyz);

	// 物体表面の材質に定数バッファーの情報をセット
	Material material = constantMaterial;

	float3 specular = lerp(0.08f * material.specular, material.baseColor, material.metallic);

	// 入射光に定数バッファーの平行光源をセット
	IncidentLight incidentLight = constantDirectionalLight;

	float3 BRDF = DiffuseBRDF(material.baseColor) * (1 - material.metallic) +
		SpecularBRDF(incidentLight, worldNormal, worldViewDir, specular, material);

	// コサイン項を計算
	float dotNL = saturate(dot(worldNormal, incidentLight.direction));
	// 放射輝度とコサイン項を掛けて放射照度に変換
	float3 irradiance = dotNL * incidentLight.color;
	// irradianceにπを掛けて調整
	irradiance *= PI;

	// レンダリング方程式より反射成分 = BRDF * 放射照度(放射輝度 * コサイン項)
	float3 Radiance = BRDF * irradiance;

	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz* Radiance, texel.w*1.0f);
	// return float4(outgoingLight, opacity);
}
