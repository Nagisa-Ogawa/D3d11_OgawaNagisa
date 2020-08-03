
// 平行光源用構造体
struct ConstantDirectionalLight {
	float4 direction;
	float4 color;
};

// 点光源用構造体
struct ConstantPointLight {
	float4 position;
	float4 color;
	float4 distance_decay;
};

// 物体表面の質感用構造体
struct MaterialTexture {
	float4 roughness_metallic;
	float4 albedo;
};

// 定数バッファー
cbuffer ConstantBuffer
{
	// ワールド変換行列
	float4x4 World;
	// ビュー変換行列
	float4x4 View;
	// 透視射影変換行列
	float4x4 Projection;
	// WVP行列
	float4x4 WorldViewProjection;
	//視点座標
	float4 viewPosition;
	// 平行光源情報
	ConstantDirectionalLight constantDirectionalLight;
	// 点光源情報
	ConstantPointLight constantPointLight;
	// 物体表面の質感の情報
	MaterialTexture materialTexture;
}

// 頂点シェーダ―への入力
struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// 頂点シェーダーからの出力
struct VSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 worldNormal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ジオメトリーシェーダーへの入力
typedef VSOutput GSInput;

// ジオメトリーシェーダーからの出力
struct GSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 worldNormal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ピクセルシェーダ―への入力
typedef GSOutput PSInput;
