
//// 平行光源用構造体
//struct ConstantDirectionalLight {
//	float4 direction;
//	float4 color;
//};
//
//// 物体表面の質感用構造体
//struct ConstantMaterial {
//	float3 baseColor;
//	float metallic;
//	float3 specular;
//	float roughness;
//};
//
//// 定数バッファー
//cbuffer ConstantBuffer
//{
//	// ワールド変換行列
//	float4x4 World;
//	// ビュー変換行列
//	float4x4 View;
//	// 透視射影変換行列
//	float4x4 Projection;
//	// WVP行列
//	float4x4 WorldViewProjection;
//	//視点座標
//	float4 viewPosition;
//	// 平行光源情報
//	ConstantDirectionalLight constantDirectionalLight;
//	// 物体表面の質感の情報
//	ConstantMaterial constantMaterial;
//}
//
//// 頂点シェーダ―への入力
//struct VSInput
//{
//	float4 position : POSITION;
//	float3 normal : NORMAL;
//	float2 texCoord : TEXCOORD;
//};
//
//// 頂点シェーダーからの出力
//struct VSOutput
//{
//	float4 position : SV_POSITION;
//	float4 worldPosition : POSITION;
//	float3 worldNormal : NORMAL;
//	float2 texCoord : TEXCOORD;
//};
//
//// ジオメトリーシェーダーへの入力
//typedef VSOutput GSInput;
//
//// ジオメトリーシェーダーからの出力
//struct GSOutput
//{
//	float4 position : SV_POSITION;
//	float4 worldPosition : POSITION;
//	float3 worldNormal : NORMAL;
//	float2 texCoord : TEXCOORD;
//};
//
//// ピクセルシェーダ―への入力
//typedef GSOutput PSInput;
// 頂点シェーダーの入力
struct VertexShaderInput
{
	float4 position : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord  : TEXCOORD;
};

// ジオメトリー シェーダーの入力
struct GeometryShaderInput
{
	float4 position      : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 worldNormal   : NORMAL;
	float2 texCoord  : TEXCOORD;
};

// ピクセル シェーダーの入力
struct PixelShaderInput
{
	float4 position      : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 worldNormal   : NORMAL;
	float2 texCoord  : TEXCOORD;
};
