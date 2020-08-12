//=============================================================================
// Material.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
std::shared_ptr<Material> Material::Create(
	std::shared_ptr<VertexShader> vertexShader,
	std::shared_ptr<GeometryShader> geometryShader,
	std::shared_ptr<PixelShader> pixelShader)
{
	return shared_ptr<Material>(
		new Material(vertexShader, geometryShader, pixelShader));
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Material::Material(
	std::shared_ptr<VertexShader> vertexShader,
	std::shared_ptr<GeometryShader> geometryShader,
	std::shared_ptr<PixelShader> pixelShader)
{
	this->vertexShader = vertexShader;
	this->geometryShader = geometryShader;
	this->pixelShader = pixelShader;
}

// ���_�V�F�[�_�[
std::shared_ptr<VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

// �W�I���g���[ �V�F�[�_�[
std::shared_ptr<GeometryShader> Material::GetGeometryShader()
{
	return geometryShader;
}

// �s�N�Z�� �V�F�[�_�[
std::shared_ptr<PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}
