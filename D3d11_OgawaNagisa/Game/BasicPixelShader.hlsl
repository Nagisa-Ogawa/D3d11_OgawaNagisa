#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// 円周率

// 物体の情報
struct Material {
	float3 diffuseRatioColor;		// 入射光のうち拡散反射になる割合
	float3 specularRatioColor;		// 入射光のうち鏡面反射になる割合
	float roughness;	// 物体表面の粗さ
};

// 入射光(光源から入ってくる光)
struct IncidentLight {
	float4 direction;	// 入射してくる向き
	float4 color;		// 光の波長 = 色
};

// 拡散反射BRDF(正規化ランバートモデル)
// fd = pd/π
float3 DiffuseBRDF(float3 diffuseColor) {
	return diffuseColor / PI;
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
(IncidentLight incidentLight, float3 worldNormal, float3 worldViewDir,
	float3 specularColor, float roughness) {

	float3 lightDirection = incidentLight.direction;	// ライトベクトル
	float3 halfVec = normalize(lightDirection + worldViewDir);		// ハーフベクトル

	float dotNL = saturate(dot(worldNormal,lightDirection ));	// 法線とライトベクトルの内積
	float dotNV = saturate(dot(worldNormal, worldViewDir));	// 法線と視線ベクトルの内積	
	float dotNH = saturate(dot(worldNormal, halfVec));	// 法線とハーフベクトルの内積
	float dotVH = saturate(dot(worldViewDir, halfVec));	// 視線ベクトルとハーフベクトルの内積
	float dotLV = saturate(dot(lightDirection, worldViewDir));	// ライトベクトルと視線ベクトルの内積
	float a = roughness * roughness;	// ラフネスの二乗

	// D項(GGX)
	float D = D_GGX(a, dotNH);
	// G項(SmithモデルのSchlick近似)
	float G = G_Smith_Schlick(a, dotNV, dotNL);
	// F項(Schlickの近似式)
	float3 F = F_Schlick(specularColor, worldViewDir, halfVec);
	// f(r, s) = DGF / 4(n・l)(n・v)
	return (F * (G * D)) / (4.0 * dotNL * dotNV);
}

float4 main(PSInput input) : SV_TARGET
{
	// ワールド座標系のポジション
	float3 worldPosition = input.worldPosition.xyz;
	// 正規化したワールド座標系の法線
	float3 worldNormal = normalize(input.worldNormal);
	// 正規化したワールド座標系の視線ベクトル(物体からカメラへの)
	float3 worldViewDir = normalize(viewPosition.xyz - input.worldPosition.xyz);

	// 物体表面の材質
	Material material;
	// 金属か非金属かを設定するパラメータ = 鏡面反射率
	float metallic = 0.0f;
	// 物体表面の粗さ
	// 金属か非金属かを設定するパラメータ =
	float roughness = 0.0f;
	/*波長ごとの反射能 = 色ごとの反射能*/
	float3 albedo = float3(0.0f, 0.0f, 0.0f);
	metallic = materialTexture.roughness_metallic.y;
	albedo = materialTexture.albedo.xyz;
	roughness = materialTexture.roughness_metallic.x;

	// albedo～float3(0.0,0.0,0.0)を0～1としたうちのmetallic部分
	material.diffuseRatioColor = lerp(albedo, float3(0.0,0.0,0.0), metallic);
	// float3(0.04,0.04,0.04)～albedoを0～1としたうちのmetallic部分
	material.specularRatioColor = lerp(float3(0.04,0.04,0.04), albedo, metallic);
	material.roughness = roughness;

	float3 reflectedDiffuse= float3(0.0, 0.0, 0.0);
	float3 reflectedSpecular = float3(0.0, 0.0, 0.0);

	// 	自己放射成分
	float3 emissive = float3(0.0,0.0,0.0);
	float opacity = 1.0;


	// 入射光に定数バッファーの平行光源をセット
	IncidentLight incidentLight = constantDirectionalLight;

	// コサイン項を計算
	float dotNL = saturate(dot(worldNormal, incidentLight.direction));
	// 放射輝度とコサイン項を掛けて放射照度に変換
	float3 irradiance = dotNL * incidentLight.color;
	// irradianceにπを掛けて調整
	irradiance *= PI;

	// レンダリング方程式より反射成分 = BRDF * 放射照度(放射輝度 * コサイン項)
	// 拡散反射成分
	reflectedDiffuse += DiffuseBRDF(material.diffuseRatioColor) * irradiance;
	// 鏡面反射成分
	reflectedSpecular += 
		SpecularBRDF(incidentLight, worldNormal, worldViewDir, material.specularRatioColor, material.roughness) * irradiance;
	// 放射輝度 = 自己放射　+ 反射成分(直接光の拡散反射成分 + 直接光の鏡面反射成分)
	float3 Radiance = emissive + reflectedDiffuse + reflectedSpecular;
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz* Radiance, texel.w*opacity);
	// return float4(outgoingLight, opacity);
}

