//=============================================================================
// Renderer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;
using namespace DirectX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
std::shared_ptr<MeshRenderer> MeshRenderer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material)
{
	// D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT

	// ���_�f�[�^�ɂ��ē��͗v�f���`
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// ���̓��C�A�E�g���쐬
	auto inputLayout = InputLayout::Create(graphicsDevice,
		inputElementDescs, material->GetVertexShader()->GetShaderBytecode());

	return shared_ptr<MeshRenderer>(
		new MeshRenderer(mesh, material, inputLayout));
}

// �C���X�^���X�������֎~
MeshRenderer::MeshRenderer(
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material,
	std::shared_ptr<InputLayout> inputLayout)
{
	this->mesh = mesh;
	this->material = material;
	this->inputLayout = inputLayout;
}

// ���̃����_���[�ɂ��`������s���܂��B
void MeshRenderer::Draw(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext)
{
	// ���̓A�Z���u���[�ɒ��_�o�b�t�@�[��ݒ�
	immediateContext->IASetVertexBuffers(
		0, mesh->GetNumBuffers(),
		mesh->GetNativePointers(), mesh->GetStrides(), mesh->GetOffsets());
	// ���̓A�Z���u���[�ɃC���f�b�N�X �o�b�t�@�[��ݒ�
	auto indexBuffer = mesh->GetIndexBuffer();
	immediateContext->IASetIndexBuffer(
		indexBuffer->GetNativePointer().Get(),
		indexBuffer->GetFormat(), indexBuffer->GetOffset());
	// �v���~�e�B�u�̎�ނ�ݒ�
	immediateContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());
	// ���̓��C�A�E�g��ݒ�
	immediateContext->IASetInputLayout(inputLayout->GetNativePointer().Get());

	// �V�F�[�_�[��ݒ�
	immediateContext->VSSetShader(material->GetVertexShader()->GetNativePointer().Get(), NULL, 0);
	immediateContext->GSSetShader(material->GetGeometryShader()->GetNativePointer().Get(), NULL, 0);
	immediateContext->PSSetShader(material->GetPixelShader()->GetNativePointer().Get(), NULL, 0);

	// �`��
	UINT indexCount = indexBuffer->GetCount();
	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;
	immediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

// �����_�����O�Ɏg�p���郁�b�V�����擾���܂��B
std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
	return mesh;
}

// �����_�����O�Ɏg�p����}�e���A�����擾���܂��B
std::shared_ptr<Material> MeshRenderer::GetMaterial()
{
	return material;
}
