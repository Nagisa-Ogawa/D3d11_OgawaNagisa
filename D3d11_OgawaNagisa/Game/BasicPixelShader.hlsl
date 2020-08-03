#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

#define PI 3.14159265359	// 円周率
#define EPSILON 1e-6

// 幾何情報
struct GeometricContext {
	float3 position;	// 位置
	float3 normal;		// 法線
	float3 viewDir;		// 視線への向き
};

// 入射光(光源から入ってくる光)
struct IncidentLight {
	float3 direction;	// 入射してくる向き
	float3 color;		// 光の波長 = 色
	bool visible;		// 減衰する光が届くかどうかのフラグ
};

// 平行光源
struct DirectionalLight {
	float3 direction;	// 光源の向き
	float3 color;		// 光の色
};

// 点光源
struct PointLight {
	float3 position;	// 光源の位置
	float3 color;		// 光の色
	float distance;		// 光源から光が届く距離
	float decay;		// 減衰率
};

// スポットライト
struct SpotLight {
	float3 position;	// 光源の位置
	float3 direction;	// 光源の向き
	float3 color;		// 光の色
	float distance;		// 光源から光が届く距離
	float decay;		// 減衰率
	float coneCos;		// 光源から出射する光線の幅
	float penumbraCos;	// 光源から出射する光線の減衰幅
};

// 物体の情報
struct Material {
	float3 diffuseColor;		// 入射光のうち拡散反射になる割合
	float3 specularColor;		// 入射光のうち鏡面反射になる割合
	float specularRoughness;	// 物体表面の粗さ
};

// 反射光
struct ReflectedLight {
	float3 directDiffuse;		// 直接光の拡散反射成分
	float3 directSpecular;		// 直接光の鏡面反射成分
	float3 indirectDiffuse;		// 間接光の拡散反射成分
	float3 indirectSpecular;	// 間接光の鏡面反射成分
};

// 光源から光が届くかどうかチェックする関数
// lightDistance = 物体から光源までの距離
// cutoffDistance = 光源から光が届く距離
bool CheckLightRange(float lightDistance, float cutoffDistance) {
	// 光源から光が届く距離が0
	// もしくは, 物体から光源までの距離より光源から光が届く距離のほうが長い
	// どちらかがtrueならtrue
	return any(bool2(cutoffDistance == 0.0, lightDistance < cutoffDistance));
}

// 光源からの減衰率を計算する関数
// lightDistance = 物体から光源までの距離
// cutoffDistance = 光源から光が届く距離
// decayExponent = 光源の減衰率
float DecayFromLightSource
(float lightDistance, float cutoffDistance, float decayExponent) {
	// 減衰率が0より大きいなら
	if (decayExponent > 0.0) {
		// (-物体から光源までの距離 / 光源から光が届く距離 + 1.0)の光源の減衰率 乗
		return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
	}

	// 減衰率が0なら1.0にする
	return 1.0;
}

// 平行光源からの入射光の情報を取得する関数
// directionalLight = 平行光源
// geometry = 物体の幾何情報
// directLight = 入射光(参照渡し)
void getDirectionalDirectLightIrradiance
(DirectionalLight directionalLight,
	GeometricContext geometry, out IncidentLight directLight) {
	// 入射光の色を平行光源の色にする
	directLight.color = directionalLight.color;
	// 入射光の向きを平行光源の向きにする
	directLight.direction = directionalLight.direction;
	directLight.visible = true;
}

// 点光源からの入射光の情報を取得する関数
// pointLight = 平行光源
// geometry = 物体の幾何情報
// directLight = 入射光(参照渡し)
void getPointDirectLightIrradiance
(PointLight pointLight, GeometricContext geometry, out IncidentLight directLight) {
	// 光源から物体までの距離
	float3 L = pointLight.position - geometry.position;
	// 正規化した光の方向
	directLight.direction = normalize(L);

	// 光の長さ
	float lightDistance = length(L);
	// 光が届いているかチェック
	if (CheckLightRange(lightDistance, pointLight.distance)) {
		directLight.color = pointLight.color;
		// 減衰を計算
		directLight.color *= DecayFromLightSource(lightDistance, pointLight.distance, pointLight.decay);
		directLight.visible = true;
	}
	// 届いていないなら計算に含まない
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

	float3 N = geometry.normal;		// 法線
	float3 V = geometry.viewDir;	// 視線ベクトル
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

// RenderEquations(レンダリング方程式)
// レンダリング方程式を計算する関数
void RenderEquations_Direct
(IncidentLight directLight, GeometricContext geometry,
	Material material, inout ReflectedLight reflectedLight) {

	// コサイン項を計算
	float dotNL = saturate(dot(geometry.normal, directLight.direction));
	// 放射輝度とコサイン項を掛けて放射照度に変換
	float3 irradiance = dotNL * directLight.color;

	// punctuallightにπを掛けて調整
	irradiance *= PI;

	// レンダリング方程式よりBRDF*放射輝度*コサイン項
	// 拡散反射成分
	reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
	// 鏡面反射成分
	reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

float4 main(PSInput input) : SV_TARGET
{
	{
		//float4 worldPosition = input.worldPosition;
		//// 面から光源を指す正規化ベクトルL(平行光源(w=0)と点光源(w=1))
		//float3 light = normalize(LightPosition.xyz - LightPosition.w * worldPosition.xyz);

		//// ワールド空間上の法線ベクトルN
		//float3 worldNormal = input.worldNormal;
		//// 拡散反射
		//float diffuse = max	// 大きいほうの値を使用
		//(
		//	dot(light, worldNormal),	// 光源と法線ベクトルの内積
		//	0							// マイナス以下なら0
		//);
		//float3 diffuseColor = diffuse * MaterialDiffuse.xyz;

		//// 鏡面反射

		//// 正反射方向
		//// 正反射方向のベクトル = 2 * 法線ベクトル * 法線ベクトルと面から光源を指す正規化ベクトルの内積 - 面から光源を指す正規化ベクトル
		//float3 reflect = 2 * input.worldNormal * dot(input.worldNormal, light) - light;
		//// 視点方向
		//// 視点方向へのベクトル = 正規化した(カメラの位置座標 - ワールド空間上の位置座標).xyz
		//float3 viewDir = normalize(ViewPosition - input.worldPosition).xyz;
		//// specular = (clamp(正反射方向と視点方向の内積))のMaterialSpecularPower(鏡面の強さ)乗
		//float specular = pow(saturate(dot(reflect, viewDir)), MaterialSpecularPower);
		//// specularColor = specular * 鏡面反射カラー
		//float3 specularColor = specular * MaterialSpecularColor.xyz;

		//float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);

		//// 拡散反射カラー + 鏡面反射カラー
		//return float4(texel.xyz * diffuseColor + specularColor, MaterialDiffuse.w * texel.w);
	}

	{
		//// ワールド空間上の座標
		//float4 worldPosition = input.worldPosition;
		//// ワールド空間上の法線ベクトルN
		//float3 worldNormal = input.worldNormal;
		//// 視点方向
		//// 視点方向へのベクトル = 正規化した(カメラの位置座標 - ワールド空間上の位置座標).xyz
		//float3 viewDir = normalize(eyePosition.xyz - worldPosition.xyz);
		//// 面から光源を指す正規化ベクトルL(平行光源(w=0)と点光源(w=1))
		//float3 l = light.lightPosition.xyz - light.lightPosition.w * worldPosition.xyz;
		//float3 halfVec = normalize(l + viewDir);
		////光源距離
		//l = normalize(l);
		////// 正反射方向
		////// 正反射方向のベクトル = 2 * 法線ベクトル * 法線ベクトルと面から光源を指す正規化ベクトルの内積 - 面から光源を指す正規化ベクトル
		//float3 reflect = 2.0 * worldNormal * dot(worldNormal, l) - l;

		//// 微小面法線分布関数
		//// GGXを使用
		//// D(h) = a^2 / π((n・h)^2(a2-1)+1)^2
		//float a = material.roughness.x;
		//float D = a*a / PI * pow(dot(worldNormal, halfVec), 2.0f) * pow((a*a - 1) + 1, 2.0f);
		//		

		//float3 iA;
		//float3 iD;
		//float3 iS;
		//// 環境反射
		//iA = material.materialAmbient.xyz * ambient.xyz;
		//// 拡散反射
		//// 光源を指す正規化ベクトルと法線ベクトルとの内積 * 鏡面反射カラー
		//iD = (max(dot(l, worldNormal), 0) * (1.0f / PI)) * material.materialDiffuse.xyz * light.lightDiffuse.xyz;
		//// 鏡面反射
		//// specular = (clamp(正反射方向と視点方向の内積))のMaterialSpecularPower(鏡面の強さ)乗 * 鏡面反射カラー
		//iS = pow(saturate(dot(reflect, viewDir)), material.materialSpecular.w) * material.materialSpecular.xyz * light.lightSpecular.xyz;

		//return float4(saturate(iA + iD + iS), 1.0);
	}

	// 物体の幾何情報
	GeometricContext geometry;
	// - (カメラの座標 - ワールド空間上の位置座標) = カメラ座標系の位置座標
	geometry.position = -(viewPosition.xyz - input.worldPosition.xyz);
	// 正規化したワールド空間上の法線
	geometry.normal = normalize(input.worldNormal);
	// 正規化した視線ベクトル = カメラの座標 - ワールド空間上の位置座標
	geometry.viewDir = normalize(viewPosition - input.worldPosition).xyz;

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

	// 
	float3 emissive = float3(0.0,0.0,0.0);
	// 自己放射成分(定数)
	float opacity = 1.0;

	// 入射光
	IncidentLight directLight;

	// 平行光源
	DirectionalLight directionalLight;
	// 点光源
	PointLight pointLight;

	//各光源の初期化
	directionalLight.direction = constantDirectionalLight.direction.xyz;
	directionalLight.color = constantDirectionalLight.color.xyz;

	pointLight.position = constantPointLight.position.xyz;
	pointLight.color = constantPointLight.color.xyz;
	pointLight.distance = constantPointLight.distance_decay.x;
	pointLight.decay = constantPointLight.distance_decay.y;

	// point light
	// ライトの数だけ計算
	// for (int i = 0; i < LIGHT_MAX; ++i) {
		// if (i >= numPointLights) break;
	// 平行光源の情報と幾何情報から入射光のパラメータを設定
	getPointDirectLightIrradiance(pointLight, geometry, directLight);
	// 光が届くなら
	if (directLight.visible) {
		// レンダリング方程式から計算
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
	// レンダリング方程式から計算
	RenderEquations_Direct(directLight, geometry, material, reflectedLight);
	// }

	// 放射輝度 = 自己放射　+ 反射成分(直接光の拡散反射成分 + 直接光の鏡面反射成分 + 間接光の拡散反射成分 + 間接光の鏡面反射成分)
	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz*outgoingLight, texel.w*opacity);
	// return float4(outgoingLight, opacity);
}

