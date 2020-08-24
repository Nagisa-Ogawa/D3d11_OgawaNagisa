//=============================================================================
// Renderer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// �C���X�^���X�������֎~
MeshRenderer::MeshRenderer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material,
	std::shared_ptr<Texture2D> texture2D)
{
	OnInitialize(graphicsDevice, mesh, material,texture2D);
}

// �������̍ۂɌĂяo����܂��B
void MeshRenderer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material,
	std::shared_ptr<Texture2D> texture2D)
{
	this->mesh = mesh;
	this->material = material;
	this->texture2D = texture2D;
	// ���̓��C�A�E�g���쐬
	inputLayout.reset(
		new InputLayout(graphicsDevice,
			mesh->GetInputElementDescs(), mesh->GetNumElements(), material->GetVertexShader()->GetShaderBytecode()));
}

// ���̃����_���[�ɂ��`������s���܂��B
void MeshRenderer::Draw(
	ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, texture2D->GetShaderResourceView().GetAddressOf());
	deviceContext->PSSetSamplers(0, 1, texture2D->GetSamplerState().GetAddressOf());
	// ���̓A�Z���u���[�ɒ��_�o�b�t�@�[��ݒ�
	deviceContext->IASetVertexBuffers(
		0, mesh->GetNumBuffers(),
		mesh->GetNativePointers(), mesh->GetStrides(), mesh->GetOffsets());
	// ���̓A�Z���u���[�ɃC���f�b�N�X �o�b�t�@�[��ݒ�
	auto indexBuffer = mesh->GetIndexBuffer();
	deviceContext->IASetIndexBuffer(
		indexBuffer->GetNativePointer().Get(),
		indexBuffer->GetFormat(), indexBuffer->GetOffset());
	// �v���~�e�B�u�̎�ނ�ݒ�
	deviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());
	// ���̓��C�A�E�g��ݒ�
	deviceContext->IASetInputLayout(inputLayout->GetNativePointer().Get());

	// �V�F�[�_�[��ݒ�
	deviceContext->VSSetShader(material->GetVertexShader()->GetNativePointer().Get(), NULL, 0);
	deviceContext->GSSetShader(material->GetGeometryShader()->GetNativePointer().Get(), NULL, 0);
	deviceContext->PSSetShader(material->GetPixelShader()->GetNativePointer().Get(), NULL, 0);

	// �`��
	UINT indexCount = indexBuffer->GetCount();
	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;
	deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

// �����_�����O�Ɏg�p���郁�b�V�����擾���܂��B
std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
	return mesh;
}

// �����_�����O�Ɏg�p���郁�b�V�����擾���܂��B
std::shared_ptr<const Mesh> MeshRenderer::GetMesh() const
{
	return mesh;
}

// �����_�����O�Ɏg�p����}�e���A�����擾���܂��B
std::shared_ptr<Material> MeshRenderer::GetMaterial()
{
	return material;
}

// �����_�����O�Ɏg�p����}�e���A�����擾���܂��B
std::shared_ptr<const Material> MeshRenderer::GetMaterial() const
{
	return material;
}
