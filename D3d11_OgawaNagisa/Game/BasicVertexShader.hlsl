//#include "BasicShader.hlsli"
//
//VSOutput main(VSInput input)
//{
//	VSOutput output;
//	//// WVP変換
//	output.position = mul(input.position, WorldViewProjection);
//	output.worldPosition = mul(input.position, World);
//	output.worldNormal = normalize(mul(input.normal, (float3x3)World));
//	output.texCoord = input.texCoord;
//
//	return output;
//}

#include <BasicShader.hlsli>

cbuffer ConstantBufferForCamera : register(b0)
{
	matrix view;		// ビュー変換行列
	matrix projection;	// プロジェクション変換行列
};

cbuffer ConstantBufferForPerFrame : register(b1)
{
	matrix world;	// ワールド変換行列
};

GeometryShaderInput main(VertexShaderInput input)
{
	GeometryShaderInput output;
	// 位置座標をプロジェクション空間へ変換
	float4 worldPosition = mul(world, input.position);
	float4 viewPosition = mul(view, worldPosition);
	output.position = mul(projection, viewPosition);
	// 位置座標をワールド空間へ変換
	output.worldPosition = mul(world, input.position).xyz;
	// 法線ベクトルをビュー空間へ変換
	float3 worldNormal = mul((float3x3)world, input.normal);
	float4 viewNormal = mul(view, float4(worldNormal.xyz, 0.0f));
	output.worldNormal = normalize(worldNormal);
	output.texCoord = input.texCoord;
	return output;
}
