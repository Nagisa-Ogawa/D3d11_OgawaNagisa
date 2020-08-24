//=============================================================================
// Material.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

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

// ���_�V�F�[�_�[���擾���܂��B
std::shared_ptr<VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

// ���_�V�F�[�_�[���擾���܂��B
std::shared_ptr<const VertexShader> Material::GetVertexShader() const
{
	return vertexShader;
}

// �W�I���g���[ �V�F�[�_�[���擾���܂��B
std::shared_ptr<GeometryShader> Material::GetGeometryShader()
{
	return geometryShader;
}

// �W�I���g���[ �V�F�[�_�[���擾���܂��B
std::shared_ptr<const GeometryShader> Material::GetGeometryShader() const
{
	return geometryShader;
}

// �s�N�Z�� �V�F�[�_�[���擾���܂��B
std::shared_ptr<PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

// �s�N�Z�� �V�F�[�_�[���擾���܂��B
std::shared_ptr<const PixelShader> Material::GetPixelShader() const
{
	return pixelShader;
}
