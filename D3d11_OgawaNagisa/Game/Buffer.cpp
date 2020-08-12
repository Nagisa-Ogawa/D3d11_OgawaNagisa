//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<VertexBuffer> VertexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth)
{
	// ���_�o�b�t�@�[�ɂ��Ă̋L�q
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
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;
	// ���_�o�b�t�@�[���쐬
	ComPtr<ID3D11Buffer> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));

	return shared_ptr<VertexBuffer>(
		new VertexBuffer(graphicsDevice, nativePointer));
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexBuffer::VertexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth, DXGI_FORMAT format)
{
	// ���_�o�b�t�@�[�ɂ��Ă̋L�q
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
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;
	// ���_�o�b�t�@�[���쐬
	ComPtr<ID3D11Buffer> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));

	UINT count = 0;
	switch (format) {
	case DXGI_FORMAT_R32_UINT:
		count = byteWidth / sizeof(UINT);
		break;
	case DXGI_FORMAT_R16_UINT:
		count = byteWidth / sizeof(USHORT);
		break;
	default:
		throw DX::com_exception(E_INVALIDARG);
		return nullptr;
	}
	return shared_ptr<IndexBuffer>(
		new IndexBuffer(graphicsDevice, nativePointer, format, count));
}

// ���̃N���X�̃C���X�^���X�����������܂��B
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer,
	DXGI_FORMAT format, UINT count)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
	this->format = format;
	this->count = count;
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
