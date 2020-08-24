//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexBuffer::VertexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, byteWidth);
}

// �������̍ۂɌĂяo����܂��B
void VertexBuffer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth)
{
	// �o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �������f�[�^�ɂ��Ă̋L�q
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = initialData;
	// �o�b�t�@�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth, DXGI_FORMAT format)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, byteWidth, format);
}

// �������̍ۂɌĂяo����܂��B
void IndexBuffer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth, DXGI_FORMAT format)
{
	// �o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �������f�[�^�ɂ��Ă̋L�q
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = initialData;
	// �o�b�t�@�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));

	this->format = format;
	this->offset = 0;

	switch (format) {
	case DXGI_FORMAT_R32_UINT:
		count = byteWidth / sizeof(UINT);
		break;
	case DXGI_FORMAT_R16_UINT:
		count = byteWidth / sizeof(USHORT);
		break;
	default:
		throw DX::ComException(E_INVALIDARG);
		break;
	}
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// �C���f�b�N�X �o�b�t�@�[���̗v�f�̃t�H�[�}�b�g���擾���܂��B
DXGI_FORMAT IndexBuffer::GetFormat() const
{
	return format;
}

// �C���f�b�N�X �o�b�t�@�[�擪����̃I�t�Z�b�g���擾���܂��B
UINT IndexBuffer::GetOffset() const
{
	return offset;
}

// �C���f�b�N�X �o�b�t�@�[���̗v�f�̐����擾���܂��B
UINT IndexBuffer::GetCount() const
{
	return count;
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
ConstantBuffer::ConstantBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, byteWidth);
}

// �������̍ۂɌĂяo����܂��B
void ConstantBuffer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth)
{
	// �o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = byteWidth;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;
	// �o�b�t�@�[���쐬
	ThrowIfFailed(graphicsDevice->GetDevice()->CreateBuffer(
		&constantBufferDesc, NULL, &nativePointer));
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer::GetNativePointer() const
{
	return nativePointer;
}
