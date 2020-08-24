//=============================================================================
// Material.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

// このクラスの新しいインスタンスを初期化します。
Material::Material(
	std::shared_ptr<VertexShader> vertexShader,
	std::shared_ptr<GeometryShader> geometryShader,
	std::shared_ptr<PixelShader> pixelShader)
{
	this->vertexShader = vertexShader;
	this->geometryShader = geometryShader;
	this->pixelShader = pixelShader;
}

// 頂点シェーダーを取得します。
std::shared_ptr<VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

// 頂点シェーダーを取得します。
std::shared_ptr<const VertexShader> Material::GetVertexShader() const
{
	return vertexShader;
}

// ジオメトリー シェーダーを取得します。
std::shared_ptr<GeometryShader> Material::GetGeometryShader()
{
	return geometryShader;
}

// ジオメトリー シェーダーを取得します。
std::shared_ptr<const GeometryShader> Material::GetGeometryShader() const
{
	return geometryShader;
}

// ピクセル シェーダーを取得します。
std::shared_ptr<PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

// ピクセル シェーダーを取得します。
std::shared_ptr<const PixelShader> Material::GetPixelShader() const
{
	return pixelShader;
}
