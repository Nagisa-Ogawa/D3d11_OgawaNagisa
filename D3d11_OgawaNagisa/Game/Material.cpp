//=============================================================================
// Material.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

// このクラスの新しいインスタンスを作成します。
std::shared_ptr<Material> Material::Create(
	std::shared_ptr<VertexShader> vertexShader,
	std::shared_ptr<GeometryShader> geometryShader,
	std::shared_ptr<PixelShader> pixelShader)
{
	return shared_ptr<Material>(
		new Material(vertexShader, geometryShader, pixelShader));
}

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

// 頂点シェーダー
std::shared_ptr<VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

// ジオメトリー シェーダー
std::shared_ptr<GeometryShader> Material::GetGeometryShader()
{
	return geometryShader;
}

// ピクセル シェーダー
std::shared_ptr<PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}
